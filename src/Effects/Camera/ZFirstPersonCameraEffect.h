#pragma once
#include "Effects/Base/ZCameraEffectBase.h"
#include "Effects/Base/ZInterpolatingEffectBase.h"

class ZSpatialEntity;

class ZFirstPersonCameraEffect : public ZCameraEffectBase, public ZInterpolatingEffectBase
{
public:
    ZFirstPersonCameraEffect() :
        ZCameraEffectBase(),
        ZInterpolatingEffectBase(1.0f)
    {
    }

    void OnClearScene() override;
    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "First Person Camera";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

private:
    TEntityRef<ZSpatialEntity> m_rPlayerHeadAttachEntity;
};
