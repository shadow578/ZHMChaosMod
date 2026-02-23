#pragma once

#include "IChaosEffect.h"

class ZSlowTimeScaleEffect : public IChaosEffect
{
public:
    ZSlowTimeScaleEffect(const float32 p_fTargetTimeScale) : m_fTargetTimeScale(p_fTargetTimeScale) {}

    void Start() override;
    void Stop() override;

    std::string GetName() const override;
    std::string GetDisplayName(const bool p_bVoting) const override;
    EDuration GetDuration() const override;

private:
    const float32 m_fTargetTimeScale = 0.2f;
    float32 m_fPreviousTimeScale = 1.0f;
};