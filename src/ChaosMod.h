#pragma once

#include "IPluginInterface.h"

#include <Glacier/ZScene.h>

#include "Helpers/ZTimer.h"
#include "IChaosEffect.h"

#include <vector>
#include <queue>
#include <functional>

class ChaosMod : public IPluginInterface
{
public:
    ChaosMod();
    ~ChaosMod();

    void Init() override;
    void OnEngineInitialized() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent);
    void OnDrawMenu() override;
    void OnDrawUI(bool p_HasFocus) override;

private: // hooks
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnLoadScene, ZEntitySceneContext*, SSceneInitParameters&);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnClearScene, ZEntitySceneContext* th, bool p_FullyUnloadScene);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnSetLoadingStage, ZEntitySceneContext* th, ESceneLoadingStage stage);

private: // general UI
    void DrawUnlockersSection();

private: // misc.
    bool m_bMenuActive = false;
    ZTimer m_SlowUpdateTimer;
    std::queue<std::function<void()>> m_qDeferredFrameUpdateActions;

    void OnEffectSlowUpdate();

    void LoadEffectResources();
    void ForwardEnterScene();

    void OnLoadOrClearScene();
    float32 GetEffectRemainingTime(const IChaosEffect* p_pEffect) const;

private: // Selection & Countdown logic
    struct SActiveEffect
    {
        IChaosEffect* m_pEffect;
        float32 m_fDuration;
        float32 m_fTimeRemaining;
    };

    ZTimer m_EffectTimer;
    float32 m_fFullEffectDuration;
    int m_nVoteOptions;

    std::vector <IChaosEffect*> m_aCurrentVote;
    std::vector<SActiveEffect> m_aActiveEffects;

    void OnEffectTimerTrigger();
    void ActivateEffect(IChaosEffect* p_pEffect);
    void UpdateEffectExpiration(const float32 p_fDeltaTime);
    std::vector<IChaosEffect*> GetRandomEffectSelection(const int p_nCount);
    bool IsCompatibleWithAllActive(const IChaosEffect* p_pEffect);

private: // Debug
    void DrawDebugWindow();
    void DrawEffectDebugPane();

    bool m_bDebugMenuActive = false;
    float32 m_fDebugEffectRemainingTime = 30.0f;
    IChaosEffect* m_pEffectForDebug = nullptr;
};

DECLARE_ZHM_PLUGIN(ChaosMod)
