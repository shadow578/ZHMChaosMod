#include "ChaosMod.h"

#include "Logging.h"

#include <Glacier/ZGameTime.h>
#include <Glacier/ZGameLoopManager.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZTimer.h"
#include "Helpers/Utils.h"

#include "EffectRegistry.h"
#include "BuildInfo.h"

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

    ForeachEffect([](IChaosEffect* p_pEffect)
        {
            Logger::Debug(TAG "Forwarding OnModUnload to '{}'", p_pEffect->GetName());
            p_pEffect->OnModUnload();
        }
	);
}

void ChaosMod::Init()
{
    Logger::Info(TAG " ChaosMod Version {} ({}) loaded.",
        BuildInfo::GetVersion(),
        BuildInfo::c_sRemoteUrl
    );

    Hooks::ZEntitySceneContext_LoadScene->AddDetour(this, &ChaosMod::OnLoadScene);
    Hooks::ZEntitySceneContext_ClearScene->AddDetour(this, &ChaosMod::OnClearScene);
    Hooks::ZEntitySceneContext_SetLoadingStage->AddDetour(this, &ChaosMod::OnSetLoadingStage);

    // sort effect registry to make it more pleasing in debug ui
    EffectRegistry::GetInstance().Sort();

    InitAuthorNames();

    ForeachEffect([](IChaosEffect* p_pEffect)
        {
            Logger::Debug(TAG "Forwarding OnModInitialized to '{}'", p_pEffect->GetName());
            p_pEffect->OnModInitialized();
        }
    );
}

void ChaosMod::OnEngineInitialized()
{
    m_EffectTimer.Initialize();
    m_SlowUpdateTimer.Initialize();

    const ZMemberDelegate<ChaosMod, void(const SGameUpdateEvent&)> s_Delegate(this, &ChaosMod::OnFrameUpdate);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);

    ForeachEffect([](IChaosEffect* p_pEffect)
        {
            Logger::Debug(TAG "Forwarding OnEngineInitialized to '{}'", p_pEffect->GetName());
            p_pEffect->OnEngineInitialized();
        }
    );
}

void ChaosMod::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    ForeachEffect([this, p_UpdateEvent](IChaosEffect* p_pEffect)
        {
            p_pEffect->OnFrameUpdate(p_UpdateEvent, GetEffectRemainingTime(p_pEffect));
        }
    );

    UpdateEffectExpiration(p_UpdateEvent.m_GameTimeDelta.ToSeconds());

    while (!m_qDeferredFrameUpdateActions.empty())
    {
        const auto& s_Action = m_qDeferredFrameUpdateActions.front();
        s_Action();

        m_qDeferredFrameUpdateActions.pop();
    }
}

void ChaosMod::ForeachEffect(std::function<void(IChaosEffect* p_pEffect)> p_Callback, const bool p_bOnlyAvailable)
{
    for (auto& s_Effect : EffectRegistry::GetInstance().GetEffects())
    {
        if (!s_Effect || (p_bOnlyAvailable && !s_Effect->Available()))
        {
            continue;
        }

		p_Callback(s_Effect.get());
    }
}


void ChaosMod::OnEffectSlowUpdate()
{
    ForeachEffect([this](IChaosEffect* p_pEffect)
        {
            p_pEffect->OnSlowUpdate(m_SlowUpdateTimer.GetElapsedSeconds(), GetEffectRemainingTime(p_pEffect));
        }
    );
}

void ChaosMod::OnLoadOrClearScene()
{
    // ensure all active effects are stopped before unload
    ForeachEffect([](IChaosEffect* p_pEffect)
        {
            p_pEffect->Stop();
        }
    );

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

    ForeachEffect([](IChaosEffect* p_pEffect)
        {
            Logger::Debug(TAG "Forwarding OnClearScene to '{}'", p_pEffect->GetName());
            p_pEffect->OnClearScene();
        }
    );

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
        ForeachEffect([](IChaosEffect* p_pEffect)
            {
                Logger::Debug(TAG "Loading Resources for '{}'", p_pEffect->GetName());
                p_pEffect->LoadResources();
            },
            false // even when not available
        );
    }

    if (!s_bIsMenu && stage == ESceneLoadingStage::eLoading_ScenePlaying)
    {
        ForeachEffect([](IChaosEffect* p_pEffect)
            {
                Logger::Debug(TAG "Forwarding OnEnterScene to '{}'", p_pEffect->GetName());
                p_pEffect->OnEnterScene();
            }
        );
	}

    return HookResult<void>(HookAction::Continue());
}

DEFINE_ZHM_PLUGIN(ChaosMod);
