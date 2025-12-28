#include "ChaosMod.h"

#include "Logging.h"

#include <Glacier/ZGameTime.h>
#include <Glacier/ZGameLoopManager.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZTimer.h"
#include "Helpers/Utils.h"

#include "EffectRegistry.h"

#define TAG "[ChaosMod] "

ChaosMod::ChaosMod() :
    m_fFullEffectDuration(60.0f),
    m_nVoteOptions(4),
    m_EffectTimer(std::bind(&ChaosMod::OnEffectTimerTrigger, this), 30.0),
    m_SlowUpdateTimer(std::bind(&ChaosMod::OnEffectSlowUpdate, this), 0.2, true) // ~5 FPS
{

}

ChaosMod::~ChaosMod()
{
    Hooks::ZEntitySceneContext_LoadScene->RemoveDetour(&ChaosMod::OnLoadScene);
    Hooks::ZEntitySceneContext_ClearScene->RemoveDetour(&ChaosMod::OnClearScene);
    Hooks::ZEntitySceneContext_SetLoadingStage->RemoveDetour(&ChaosMod::OnSetLoadingStage);

    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            Logger::Debug(TAG "Forwarding OnModUnload to '{}'", s_Effect->GetName());
            s_Effect->OnModUnload();
        }
    }
}

void ChaosMod::Init()
{
    Hooks::ZEntitySceneContext_LoadScene->AddDetour(this, &ChaosMod::OnLoadScene);
    Hooks::ZEntitySceneContext_ClearScene->AddDetour(this, &ChaosMod::OnClearScene);
    Hooks::ZEntitySceneContext_SetLoadingStage->AddDetour(this, &ChaosMod::OnSetLoadingStage);

    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            Logger::Debug(TAG "Forwarding OnModInitialized to '{}'", s_Effect->GetName());
            s_Effect->OnModInitialized();
        }
    }
}

void ChaosMod::OnEngineInitialized()
{
    m_EffectTimer.Initialize();
    m_SlowUpdateTimer.Initialize();

    const ZMemberDelegate<ChaosMod, void(const SGameUpdateEvent&)> s_Delegate(this, &ChaosMod::OnFrameUpdate);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);

    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            Logger::Debug(TAG "Forwarding OnEngineInitialized to '{}'", s_Effect->GetName());
            s_Effect->OnEngineInitialized();

            if (!s_Effect->Available())
            {
                Logger::Warn(
                    TAG "'{}' reported as unavailable during OnEngineInitialized, it will not be used.",
                    s_Effect->GetName());
            }
        }
    }
}

void ChaosMod::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            s_Effect->OnFrameUpdate(p_UpdateEvent, GetEffectRemainingTime(s_Effect.get()));
        }
    }

    UpdateEffectExpiration(p_UpdateEvent.m_GameTimeDelta.ToSeconds());

    while (!m_qDeferredFrameUpdateActions.empty())
    {
        const auto& s_Action = m_qDeferredFrameUpdateActions.front();
        s_Action();

        m_qDeferredFrameUpdateActions.pop();
    }
}

void ChaosMod::OnEffectSlowUpdate()
{
    const float s_fDeltaTime = m_SlowUpdateTimer.GetElapsedSeconds();

    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            s_Effect->OnSlowUpdate(s_fDeltaTime, GetEffectRemainingTime(s_Effect.get()));
        }
    }
}

float32 ChaosMod::GetEffectRemainingTime(const IChaosEffect* p_pEffect) const
{
    // debug takes precedence
    if (p_pEffect == m_pEffectForDebug)
    {
        return m_fDebugEffectRemainingTime;
    }

    for (const auto& s_ActiveEffect : m_aActiveEffects)
    {
        if (s_ActiveEffect.m_pEffect == p_pEffect)
        {
            return s_ActiveEffect.m_fTimeRemaining;
        }
    }

    // not active
    return 0.0f;
}

void ChaosMod::LoadEffectResources()
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        // note s_Effect->Available() is not checked, as OnPreloadResources explicitly is allowed
        // to run for unavailable effects
        if (s_Effect)
        {
            Logger::Info(TAG "Loading Resources for '{}'", s_Effect->GetName());
            s_Effect->LoadResources();
        }
    }
}

void ChaosMod::ForwardEnterScene()
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            Logger::Debug(TAG "Forwarding OnEnterScene to '{}'", s_Effect->GetName());
            s_Effect->OnEnterScene();
        }
	}
}

void ChaosMod::OnLoadOrClearScene()
{
    // ensure all active effects are stopped before unload
    for (auto& s_Effect : m_aActiveEffects)
    {
        if (s_Effect.m_pEffect->Available())
        {
            s_Effect.m_pEffect->Stop();
        }

    }

    m_EffectTimer.m_bEnable = false;

    m_aCurrentVote.clear();
    m_aActiveEffects.clear();
}

DEFINE_PLUGIN_DETOUR(ChaosMod, void, OnLoadScene, ZEntitySceneContext* th, SSceneInitParameters&)
{
    OnLoadOrClearScene();
    return HookResult<void>(HookAction::Continue());
}

DEFINE_PLUGIN_DETOUR(ChaosMod, void, OnClearScene, ZEntitySceneContext* th, bool p_FullyUnloadScene)
{
    OnLoadOrClearScene();

    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (s_Effect && s_Effect->Available())
        {
            Logger::Debug(TAG "Forwarding OnClearScene to '{}'", s_Effect->GetName());
            s_Effect->OnClearScene();
        }
    }

    return HookResult<void>(HookAction::Continue());
}

DEFINE_PLUGIN_DETOUR(ChaosMod, void, OnSetLoadingStage, ZEntitySceneContext* th, ESceneLoadingStage stage)
{
    // preload resources after assets are loaded in a level.
    // loading to early (in OnEngineInit or menu screens) can cause some issues with resource loading,
    // leading to crashes.
    const std::string s_sSceneResource = th->GetSceneInitParameters().m_SceneResource.c_str();
    const bool s_bIsMenu = s_sSceneResource == "assembly:/_PRO/Scenes/Frontend/Boot.entity"
        || s_sSceneResource == "assembly:/_PRO/Scenes/Frontend/MainMenu.entity";
    if (!s_bIsMenu && stage == ESceneLoadingStage::eLoading_AssetsLoaded)
    {
        LoadEffectResources();
    }

    if (!s_bIsMenu && stage == ESceneLoadingStage::eLoading_ScenePlaying)
    {
        ForwardEnterScene();
	}

    return HookResult<void>(HookAction::Continue());
}

DEFINE_ZHM_PLUGIN(ChaosMod);
