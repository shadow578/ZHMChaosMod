#pragma once

#include "IChaosEffect.h"

class ZLagEffect : public IChaosEffect
{
public:
    ZLagEffect(const float32 p_fTargetFPS)
        : m_fTargetFPS(p_fTargetFPS)
    {
    }

    void Start() override;
    void Stop() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName() override
    {
        return "Console Experience";
    }

    EDuration GetDuration() override
    {
        return EDuration::Short; // painful!
    }

private:
    float32 m_fTargetFPS;
    bool m_bEnabled = false;
};