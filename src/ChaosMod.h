#pragma once

#include "IPluginInterface.h"

#include <Glacier/ZScene.h>

#include "Helpers/ZTimer.h"
#include "IChaosEffect.h"
#include "IVotingIntegration.h"

#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <memory>

class ZConfigurationAccessor;

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
    DECLARE_PLUGIN_DETOUR(ChaosMod, bool, OnLoadScene, ZEntitySceneContext*, SSceneInitParameters&);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnClearScene, ZEntitySceneContext* th, bool p_FullyUnloadScene);
    DECLARE_PLUGIN_DETOUR(ChaosMod, void, OnSetLoadingStage, ZEntitySceneContext* th, ESceneLoadingStage stage);

  private: // Misc.
    ZTimer m_SlowUpdateTimer;
    std::queue<std::function<void()>> m_qDeferredFrameUpdateActions;

    void ForeachEffect(const bool p_bIsLifecycleCall, std::function<void(std::shared_ptr<IChaosEffect> p_pEffect)> p_Callback);

    void OnEffectSlowUpdate();
    void OnLoadOrClearScene();

  private: // Config
    struct SEffectEnableTemplate
    {
        std::string m_sName;
        std::string m_sDescription;

        /// default enable state for effects not listed in m_mEffectEnableStates
        bool m_bDefaultEnabled;

        /// map of effect name to whether it's enabled in this template
        std::map<std::string, bool> m_mEffectEnableStates;
    };

    std::unique_ptr<ZConfigurationAccessor> m_pConfiguration;
    static std::vector<SEffectEnableTemplate> g_aEffectEnableTemplates;

    /// Load (or reload) mod and effects configuration from disk and apply it.
    void LoadConfiguration();

    /// Enable or disable all effects.
    /// A configuration reload is required for this to take effect.
    void SetAllEffectsEnabled(const bool p_bEnabled);

    /// Enable or disable the given effect.
    /// A configuration reload is required for this to take effect.
    void SetEffectEnabled(const std::shared_ptr<IChaosEffect> p_pEffect, const bool p_bEnabled);

    /// Apply the given effect enable template, enabling or disabling effects as specified by the template.
    /// A configuration reload is required for this to take effect.
    void ApplyEffectEnableTemplate(const SEffectEnableTemplate& p_Template);

  private: // UI & Debug
    bool m_bMenuActive = false;
    std::string m_sAuthorNames;

    bool m_bEffectConfigOpen = false;
    std::shared_ptr<IChaosEffect> m_pEffectForConfig = nullptr;
    int m_nSelectedConfigTemplate = 0;
    float32 m_fEffectConfigUIButtonsWidth = 250.0f;

    bool m_bDebugMenuActive = false;
    bool m_bDebugMenuAlwaysVisible = false;
    float32 m_fDebugEffectRemainingTime = 30.0f;
    std::shared_ptr<IChaosEffect> m_pEffectForDebug = nullptr;

    void InitAuthorNames();

    void DrawMainUI(const bool p_bHasFocus);
    void DrawOverlayUI(const bool p_bHasFocus);

    void DrawConfigurationContents();
    void DrawOverlayContents();
    void DrawUnlockersContents();

    void DrawEffectConfigUI(const bool p_bHasFocus);
    void DrawEffectConfigPane();

    void DrawDebugUI(const bool p_bHasFocus);
    void DrawEffectDebugPane();

  private: // Dev Tools
    bool m_bTestmodeEnabled = false;
    size_t m_nTestmodeEffectIndex = 0;
    float32 m_fTestmodeInterval = 10.0f;
    float32 m_fTestmodeTimeToNextEffect = 0.0f;

    void UpdateTestMode(const float32 p_fDeltaTime);
    void GenerateAndOpenCompatibilityMatrix();

  private: // Selection & Countdown logic
    struct SActiveEffect
    {
        std::shared_ptr<IChaosEffect> m_pEffect;
        float32 m_fDuration;
        float32 m_fTimeRemaining;
    };

    bool m_bUserEnabled = false; // user enabled chaos through UI?
    bool m_bModEnabled = false;  // mod determined chaos may be enabled?

    ZTimer m_EffectTimer;
    bool m_bEffectTimersUseRealtime;
    float32 m_fFullEffectDuration;
    int m_nVoteOptions;

    std::shared_ptr<IVotingIntegration> m_pVotingIntegration = nullptr;
    std::vector<SActiveEffect> m_aActiveEffects;

    std::shared_ptr<IVotingIntegration> GetCurrentVotingIntegration();
    std::shared_ptr<IVotingIntegration> GetDefaultVotingIntegration();

    void UpdateEffectTimerEnabled();
    void OnEffectTimerTrigger();
    void ActivateEffect(std::shared_ptr<IChaosEffect>& p_pEffect, const float32 p_fForcedDuration = -1.0f);
    void UpdateEffectExpiration(const float32 p_fDeltaTime);
    std::vector<std::shared_ptr<IChaosEffect>> GetRandomEffectSelection(const int p_nCount);
    bool IsCompatibleWithAllActive(const std::shared_ptr<IChaosEffect>& p_pEffect);
    float32 GetEffectRemainingTime(const std::shared_ptr<IChaosEffect>& p_pEffect) const;
};

DECLARE_ZHM_PLUGIN(ChaosMod)
