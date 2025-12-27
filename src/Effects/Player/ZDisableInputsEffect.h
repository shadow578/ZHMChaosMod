#pragma once
#include "IChaosEffect.h"

class ZDisableInputsEffect : public IChaosEffect
{
public:
    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName() const override
    {
        return "No Inputs";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short; // boring if too long
    }

private:
    bool m_bEnable = false;

    DECLARE_PLUGIN_DETOUR(ZDisableInputsEffect, bool, OnInputActionDigital, ZInputAction* th, int a2);
    DECLARE_PLUGIN_DETOUR(ZDisableInputsEffect, double, OnInputActionAnalog, ZInputAction* th, int a2);
};
