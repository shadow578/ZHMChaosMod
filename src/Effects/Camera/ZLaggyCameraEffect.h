#pragma once
#include "Effects/Base/ZCameraEffectBase.h"

#include <queue>

class ZLaggyCameraEffect : public ZCameraEffectBase 
{
public:
    ZLaggyCameraEffect() :
        ZCameraEffectBase()
    {
    }

    void OnDrawDebugUI() override;

    void Start() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Laggy Camera";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

private:
    std::queue<SMatrix> m_qTransformHistory;

    int m_nDelayFrames = 15;
	float32 m_fTransformApplyPercent = 0.25f;
};
