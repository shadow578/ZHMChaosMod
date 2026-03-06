#pragma once
#include "IChaosEffect.h"

#include <IPluginInterface.h> // for DECLARE_PLUGIN_DETOUR

class ZInvertControlsEffect : public IChaosEffect
{
  public:
    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Inverted Controls";
    }

  private:
    bool m_bEnable = false;

    DECLARE_PLUGIN_DETOUR(ZInvertControlsEffect, double, OnInputActionAnalog, ZInputAction* th, int a2);
};
