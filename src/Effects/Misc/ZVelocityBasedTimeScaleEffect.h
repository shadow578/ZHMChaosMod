#pragma once

#include "IChaosEffect.h"

#include "Helpers/ZVelocityTracker.h"

class ZVelocityBasedTimeScaleEffect : public IChaosEffect
{
public:
    ZVelocityBasedTimeScaleEffect();

    void Start() override;
    void Stop() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;
    void OnDrawDebugUI() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "SUPERHOT";
    }

private:
    bool m_bActive = false;
    ZVelocityTracker m_VelocityTracker;
    float32 m_fVelocityToTimeScaleFactor;
    float32 m_fPreviousTimeScale;
};