#pragma once

#include "IChaosEffect.h"

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
     * Interpolation point between 0.0 and 1.0.
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