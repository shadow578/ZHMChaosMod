#pragma once
#include "IChaosEffect.h"

#include <string_view>

#include <IPluginInterface.h> // for DECLARE_PLUGIN_DETOUR

class ZPlayerTeleportMovementEffect : public IChaosEffect
{
  public:
    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;
    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Jumper"; // (2008)
    }

    void LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration) override;
    void DrawConfigUI(ZConfigurationAccessor* p_pConfiguration) override;

  private: // config
    float32 m_fCooldownTime = 0.75f;
    float32 m_fTeleportRaycastStartOffset = 1.0f;
    float32 m_fMaxTeleportDistance = 150.0f;

  private:
    enum class EState
    {
        DISABLED,
        WAIT_FOR_INPUT,
        TELEPORT_NEXT_FRAME,
        COOLDOWN
    };
    static std::string_view StateToName(const EState p_eState);

    bool PerformTeleport();

    EState m_eState = EState::DISABLED;
    float32 m_fCooldownRemaining = 0.0f;

    DECLARE_PLUGIN_DETOUR(ZPlayerTeleportMovementEffect, double, OnInputActionAnalog, ZInputAction* th, int a2);
};
