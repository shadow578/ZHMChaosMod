#pragma once

#include "ZCameraEffectBase.h"
#include "ZInterpolatingEffectBase.h"

/**
 * Reusable base for changing camera FOV.
 * FOV is applied to an ZCameraEffectBase.
 * FOV change is smoothly interpolated in Start() and Stop().
 */
class ZCameraFOVEffectBase : public ZCameraEffectBase, public ZInterpolatingEffectBase
{
public:
    ZCameraFOVEffectBase(const float32 p_fTargetFOV) : ZInterpolatingEffectBase(1.0f), m_fTargetFOV(p_fTargetFOV)
    {
    }

    void Start() override;
    void Stop() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

private:
    float32 m_fTargetFOV;
    float32 m_fOriginalFOV = -1.0f;
};
