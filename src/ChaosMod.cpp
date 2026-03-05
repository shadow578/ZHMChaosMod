#include "ChaosMod.h"

#include "Logging.h"

#include <Glacier/ZGameTime.h>
#include <Glacier/ZGameLoopManager.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZTimer.h"
#include "Helpers/Utils.h"
#include "Helpers/CompanionMod.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#include "EffectRegistry.h"
#include "ZEffectConfigurationAccessor.h"
#include "BuildInfo.h"

#define TAG "[ChaosMod] "

ChaosMod::ChaosMod() : m_fFullEffectDuration(60.0f),
                       m_nVoteOptions(4),
                       m_EffectTimer(std::bind(&ChaosMod::OnEffectTimerTrigger, this), 30.0),
                       m_bEffectTimersUseRealtime(false),
                       m_SlowUpdateTimer(std::bind(&ChaosMod::OnEffectSlowUpdate, this), 0.2, ZTimer::ETimeMode::RealTime, true), // ~5 FPS
                       m_pConfiguration(std::make_unique<ZEffectConfigurationAccessor>(this, "ChaosMod"))
{
}

ChaosMod::~ChaosMod()
{
    Hooks::ZEntitySceneContext_LoadScene->RemoveDetour(&ChaosMod::OnLoadScene);
    Hooks::ZEntitySceneContext_ClearScene->RemoveDetour(&ChaosMod::OnClearScene);
    Hooks::ZEntitySceneContext_SetLoadingStage->RemoveDetour(&ChaosMod::OnSetLoadingStage);

    ForeachEffect(true, [](IChaosEffect* p_pEffect) {
        Logger::Debug(TAG "Forwarding OnModUnload to '{}'", p_pEffect->GetName());
        p_pEffect->OnModUnload();
    });

    m_EffectTimer.Deinitialize();
    m_SlowUpdateTimer.Deinitialize();
}

void ChaosMod::Init()
{
    Logger::Info(TAG " ChaosMod Version {} ({} on {}) loaded on ZHMModSDK {} (target {}).", BuildInfo::GetDisplayVersion(), BuildInfo::GetInternalVersion(), BuildInfo::c_sRemoteUrl, SDKVersion(), BuildInfo::c_sTargetSDKVersion);

    Hooks::ZEntitySceneContext_LoadScene->AddDetour(this, &ChaosMod::OnLoadScene);
    Hooks::ZEntitySceneContext_ClearScene->AddDetour(this, &ChaosMod::OnClearScene);
    Hooks::ZEntitySceneContext_SetLoadingStage->AddDetour(this, &ChaosMod::OnSetLoadingStage);

    // sort effect registry to make it more pleasing in debug ui
    EffectRegistry::GetInstance().Sort();

    InitAuthorNames();

    ForeachEffect(true, [this](IChaosEffect* p_pEffect) {
        Logger::Debug(TAG "Forwarding OnModInitialized to '{}'", p_pEffect->GetName());
        p_pEffect->OnModInitialized();
    });

    for (auto& s_pVotingIntegation : EffectRegistry::GetInstance().GetVotingIntegrations())
    {
        if (s_pVotingIntegation)
        {
            Logger::Debug(TAG "Initializing Voting Integration '{}'", s_pVotingIntegation->GetName());
            s_pVotingIntegation->Initialize();
        }
    }

    m_pVotingIntegration = GetDefaultVotingIntegration();

    LoadConfiguration();
}

void ChaosMod::OnEngineInitialized()
{
    m_EffectTimer.Initialize();
    m_SlowUpdateTimer.Initialize();

    const ZMemberDelegate<ChaosMod, void(const SGameUpdateEvent&)> s_Delegate(this, &ChaosMod::OnFrameUpdate);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);

    ForeachEffect(true, [](IChaosEffect* p_pEffect) {
        Logger::Debug(TAG "Forwarding OnEngineInitialized to '{}'", p_pEffect->GetName());
        p_pEffect->OnEngineInitialized();
    });
}

void ChaosMod::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    ForeachEffect(false, [this, p_UpdateEvent](IChaosEffect* p_pEffect) {
        p_pEffect->OnFrameUpdate(p_UpdateEvent, GetEffectRemainingTime(p_pEffect));
    });

    UpdateEffectExpiration(
        m_bEffectTimersUseRealtime
            ? p_UpdateEvent.m_RealTimeDelta.ToSeconds()
            : p_UpdateEvent.m_GameTimeDelta.ToSeconds()
    );

    while (!m_qDeferredFrameUpdateActions.empty())
    {
        const auto& s_Action = m_qDeferredFrameUpdateActions.front();
        s_Action();

        m_qDeferredFrameUpdateActions.pop();
    }

    UpdateTestMode(p_UpdateEvent.m_GameTimeDelta.ToSeconds());
}

void ChaosMod::ForeachEffect(const bool p_bIsLifecycleCall, std::function<void(IChaosEffect* p_pEffect)> p_Callback)
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (!s_Effect)
        {
            continue;
        }

        if (!p_bIsLifecycleCall && !s_Effect->Available())
        {
            continue;
        }

        p_Callback(s_Effect.get());
    }
}

void ChaosMod::OnEffectSlowUpdate()
{
    ForeachEffect(false, [this](IChaosEffect* p_pEffect) {
        p_pEffect->OnSlowUpdate(m_SlowUpdateTimer.GetElapsedSeconds(), GetEffectRemainingTime(p_pEffect));
    });
}

void ChaosMod::OnLoadOrClearScene()
{
    // ensure all active effects are stopped before unload
    for (auto& s_ActiveEffect : m_aActiveEffects)
    {
        if (s_ActiveEffect.m_pEffect && s_ActiveEffect.m_pEffect->Available())
        {
            s_ActiveEffect.m_pEffect->Stop();
        }
    }

    m_bModEnabled = false;
    UpdateEffectTimerEnabled();
    m_EffectTimer.Reset();

    m_aActiveEffects.clear();

    if (auto* s_pVoting = GetCurrentVotingIntegration())
    {
        s_pVoting->EndVote();
    }
}

void ChaosMod::LoadConfiguration()
{
    // load and apply mod configuration
    m_EffectTimer.m_fIntervalSeconds = m_pConfiguration->GetDouble("EffectInterval", 30.0);
    m_fFullEffectDuration = m_pConfiguration->GetDouble("FullEffectDuration", 60.0);
    m_bEffectTimersUseRealtime = m_pConfiguration->GetBool("EffectTimersUseRealtime", false);

    m_EffectTimer.m_eTimeMode = m_bEffectTimersUseRealtime ? ZTimer::ETimeMode::RealTime : ZTimer::ETimeMode::GameTime;

    // load effect configurations
    ForeachEffect(true, [this](IChaosEffect* p_pEffect) {
        Logger::Debug(TAG "Loading configuration for '{}'", p_pEffect->GetName());
        ZEffectConfigurationAccessor s_ConfigAccessor(this, p_pEffect->GetName());
        p_pEffect->LoadConfiguration(&s_ConfigAccessor);
    });
}

void ChaosMod::SetAllEffectsEnabled(const bool p_bEnabled)
{
    ForeachEffect(true, [this, p_bEnabled](IChaosEffect* p_pEffect) {
        SetEffectEnabled(p_pEffect, p_bEnabled);
    });
}

void ChaosMod::SetEffectEnabled(const IChaosEffect* p_pEffect, const bool p_bEnabled)
{
    ZEffectConfigurationAccessor s_ConfigAccessor(this, p_pEffect->GetName());
    s_ConfigAccessor.SetBool("enabled", p_bEnabled);
}

void ChaosMod::ApplyEffectEnableTemplate(const SEffectEnableTemplate& p_Template)
{
    Logger::Info(TAG "Applying Effect Enable Template '{}'", p_Template.m_sName);
    ForeachEffect(true, [this, p_Template](IChaosEffect* p_pEffect) {
        const auto& s_It = p_Template.m_mEffectEnableStates.find(p_pEffect->GetName());
        const auto s_bEnabled = s_It != p_Template.m_mEffectEnableStates.end() ? s_It->second : p_Template.m_bDefaultEnabled;
        SetEffectEnabled(p_pEffect, s_bEnabled);
    });
}

void ChaosMod::UpdateEffectTimerEnabled()
{
    const auto s_bEnable = m_bUserEnabled && m_bModEnabled;

    if (s_bEnable)
    {
        // on enable, prepare first vote
        OnEffectTimerTrigger();
    }
    else
    {
        // on disable, stop and clear active effects
        for (auto& s_ActiveEffect : m_aActiveEffects)
        {
            if (s_ActiveEffect.m_pEffect && s_ActiveEffect.m_pEffect->Available())
            {
                s_ActiveEffect.m_pEffect->Stop();
            }
        }

        m_aActiveEffects.clear();

        if (auto* s_pVoting = GetCurrentVotingIntegration())
        {
            s_pVoting->EndVote();
        }
    }

    m_EffectTimer.m_bEnable = s_bEnable;
}

IVotingIntegration* ChaosMod::GetCurrentVotingIntegration()
{
    if (!m_pVotingIntegration)
    {
        Logger::Warn(TAG "No voting integration selected, falling back to default.");
        m_pVotingIntegration = GetDefaultVotingIntegration();
    }

    return m_pVotingIntegration;
}

IVotingIntegration* ChaosMod::GetDefaultVotingIntegration()
{
    if (const auto& s_pOfflineVoting = EffectRegistry::GetInstance().GetVotingIntegrationByName("ZOfflineVoting"))
    {
        return s_pOfflineVoting.get();
    }

    throw std::runtime_error("Failed to find default voting integration!");
    return nullptr;
}

DEFINE_PLUGIN_DETOUR(ChaosMod, bool, OnLoadScene, ZEntitySceneContext* th, SSceneInitParameters&)
{
    OnLoadOrClearScene();
    return HookResult<bool>(HookAction::Continue());
}

DEFINE_PLUGIN_DETOUR(ChaosMod, void, OnClearScene, ZEntitySceneContext* th, bool p_FullyUnloadScene)
{
    OnLoadOrClearScene();

    ForeachEffect(true, [](IChaosEffect* p_pEffect) {
        Logger::Debug(TAG "Forwarding OnClearScene to '{}'", p_pEffect->GetName());
        p_pEffect->OnClearScene();
    });

    ZHMRepositoryHelper::GetInstance().Reset();

    return HookResult<void>(HookAction::Continue());
}

DEFINE_PLUGIN_DETOUR(ChaosMod, void, OnSetLoadingStage, ZEntitySceneContext* th, ESceneLoadingStage stage)
{
    // preload resources after assets are loaded in a level.
    // loading to early (in OnEngineInit or menu screens) can cause some issues with resource loading,
    // leading to crashes.
    const std::string s_sSceneResource = th->GetSceneInitParameters().m_SceneResource.c_str();
    const bool s_bIsBoot = s_sSceneResource == "assembly:/_PRO/Scenes/Frontend/Boot.entity";
    const bool s_bIsMainMenu = s_sSceneResource == "assembly:/_PRO/Scenes/Frontend/MainMenu.entity";

    if (!s_bIsBoot && stage == ESceneLoadingStage::eLoading_AssetsLoaded)
    {
        ZHMRepositoryHelper::GetInstance().Initialize();

        // companion mod info is stored in the repository, which should be available at the main menu.
        // we need to load asap so the UI can display correct info, even when accessing from the main menu.
        CompanionModUtil::LoadCompanionModInfo();

        // resource loading of effects is defined such that it should only be done for gameplay scenes, not main menu.
        // thus, we need the extra check here.
        if (!s_bIsMainMenu)
        {
            ForeachEffect(true, [](IChaosEffect* p_pEffect) {
                Logger::Debug(TAG "Loading Resources for '{}'", p_pEffect->GetName());
                p_pEffect->LoadResources();
            });
        }
    }

    if (!s_bIsBoot && !s_bIsMainMenu && stage == ESceneLoadingStage::eLoading_ScenePlaying)
    {
        ForeachEffect(true, [](IChaosEffect* p_pEffect) {
            Logger::Debug(TAG "Forwarding OnEnterScene to '{}'", p_pEffect->GetName());
            p_pEffect->OnEnterScene();
        });

        // enable chaos when starting to play
        m_bModEnabled = true;
        UpdateEffectTimerEnabled();
    }

    return HookResult<void>(HookAction::Continue());
}

DEFINE_ZHM_PLUGIN(ChaosMod);
