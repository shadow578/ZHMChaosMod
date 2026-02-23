#pragma once

#include "IChaosEffect.h"

/**
 * Reusable effect for handling smoothly interpolating in and out.
 * Provides GetInterpolationPoint(), which provides interpolating value to effects.
 * Automatically handles interpolate in on Start(), out on Stop().
 */
class ZInterpolatingEffectBase : public virtual IChaosEffect
{
public:
    void Start() override;
    void Stop() override;
    void OnClearScene() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;
    void OnDrawDebugUI() override;

protected:
    ZInterpolatingEffectBase(const float32 p_fInterpolationTime) : m_fInterpolationTime(p_fInterpolationTime)
    {
    }

    /**
     * Interpolation point (0.0 - 1.0).
     */
    inline float32 GetInterpolationPoint() const
    {
        return m_fInterpolationPoint;
    }

private:
    enum class EState
    {
        Idle,
        InterpolateIn,
        InterpolateOut
    };

    EState m_eState = EState::Idle;
    float32 m_fInterpolationTime = 1.0f;
    float32 m_fInterpolationPoint = 0.0f;
};