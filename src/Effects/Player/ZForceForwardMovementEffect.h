#pragma once
#include "IChaosEffect.h"

class ZForceForwardMovementEffect : public IChaosEffect
{
public:
    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName() override
    {
        return "Help my W key is Stuck";
    }

    EDuration GetDuration() override
    {
        return EDuration::Full; // idk, maybe too long ?
    }

private:
    bool m_bEnable = false;

    DECLARE_PLUGIN_DETOUR(ZForceForwardMovementEffect, double, OnInputActionAnalog, ZInputAction* th, int a2);
};
