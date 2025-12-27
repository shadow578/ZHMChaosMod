#pragma once
#include "IChaosEffect.h"

class ZInvertControlsEffect : public IChaosEffect
{
public:
    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName() const  override
    {
        return "Inverted Controls";
    }

private:
    bool m_bEnable = false;

    DECLARE_PLUGIN_DETOUR(ZInvertControlsEffect, double, OnInputActionAnalog, ZInputAction* th, int a2);
};
