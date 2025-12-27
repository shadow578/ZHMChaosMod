#pragma once

#include "Effects/Base/ZCameraEffectBase.h"
#include "Effects/Base/ZInterpolatingEffectBase.h"

class ZOverheadCameraEffect : public ZCameraEffectBase, public ZInterpolatingEffectBase
{
public:
    ZOverheadCameraEffect() : ZInterpolatingEffectBase(1.0f)
    {
    }

    void Start() override;
    void Stop() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName() override
    {
        return "GTA1 Mode";
    }

private:
    float32 GetMaxOverheadZAt(const SMatrix& p_Position, const float32 p_fMax);
};
