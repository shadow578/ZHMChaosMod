#pragma once

#include "IPluginInterface.h"

#include <Glacier/ZScene.h>

#include "Helpers/ZTimer.h"
#include "IChaosEffect.h"
#include "IVotingIntegration.h"

#include <vector>
#include <queue>
#include <functional>
#include <memory>

class ChaosMod : public IPluginInterface
{
public: // Plugin Interface
    ChaosMod();
    ~ChaosMod();

    void Init() override;
    void OnEngineInitialized() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent);
    void OnDrawMenu() override;
    void OnDrawUI(bool p_HasFocus) override;

private: // Hooks
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnLoadScene, ZEntitySceneContext*, SSceneInitParameters&);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnClearScene, ZEntitySceneContext* th, bool p_FullyUnloadScene);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnSetLoadingStage, ZEntitySceneContext* th, ESceneLoadingStage stage);

private: // Misc.
    ZTimer m_SlowUpdateTimer;
    std::queue<std::function<void()>> m_qDeferredFrameUpdateActions;

    void ForeachEffect(const bool m_bIsLifecycleCall, std::function<void(IChaosEffect* p_pEffect)> p_Callback);

    void OnEffectSlowUpdate();
    void OnLoadOrClearScene();

private: // UI & Debug
    bool m_bMenuActive = false;
    bool m_bDebugMenuActive = false;
    bool m_bDebugMenuAlwaysVisible = false;
    float32 m_fDebugEffectRemainingTime = 30.0f;
    IChaosEffect* m_pEffectForDebug = nullptr;
    std::string m_sAuthorNames;

	void InitAuthorNames();

	void DrawMainUI(const bool p_bHasFocus);
	void DrawOverlayUI(const bool p_bHasFocus);

	void DrawConfigurationContents();
    void DrawOverlayContents();
    void DrawUnlockersContents();

    void DrawDebugUI(const bool p_bHasFocus);
    void DrawEffectDebugPane();

private: // Selection & Countdown logic
    struct SActiveEffect
    {
        IChaosEffect* m_pEffect;
        float32 m_fDuration;
        float32 m_fTimeRemaining;
    };

    bool m_bUserEnabled = false; // user enabled chaos through UI?
    bool m_bModEnabled = false;  // mod determined chaos may be enabled?

    ZTimer m_EffectTimer;
    float32 m_fFullEffectDuration;
    int m_nVoteOptions;

    IVotingIntegration* m_pVotingIntegration = nullptr;
    std::vector<SActiveEffect> m_aActiveEffects;

    IVotingIntegration* GetCurrentVotingIntegration();
    IVotingIntegration* GetDefaultVotingIntegration();

    void UpdateEffectTimerEnabled();
    void OnEffectTimerTrigger();
    void ActivateEffect(IChaosEffect* p_pEffect);
    void UpdateEffectExpiration(const float32 p_fDeltaTime);
    std::vector<IChaosEffect*> GetRandomEffectSelection(const int p_nCount);
    bool IsCompatibleWithAllActive(const IChaosEffect* p_pEffect);
    float32 GetEffectRemainingTime(const IChaosEffect* p_pEffect) const;
};

DECLARE_ZHM_PLUGIN(ChaosMod)
