#pragma once
#include "Effects/Base/ZGravityModifierEffectBase.h"

class ZCameraViewBasedGravityEffect : public ZGravityModifierEffectBase
{
public:
    void Start() override;
    void Stop() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName() const override
    {
        return "Camera View Based Gravity";
    }

private:
    bool m_bIsActive = false;
};
