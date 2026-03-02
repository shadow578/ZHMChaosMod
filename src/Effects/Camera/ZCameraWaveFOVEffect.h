#pragma once
#include "Effects/Base/ZCameraEffectBase.h"

class ZCameraWaveFOVEffect : public ZCameraEffectBase
{
  public:
    void Start() override;

    void OnDrawDebugUI() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Zoom Zoom Cam";
    }

  private:
    float32 m_fOriginalFOV = -1.0f;
    float32 m_fTimeSinceStart = 0.0f;

    float32 m_fFrequency = 0.2f;
    float32 m_fMinFOV = 20.0f;
    float32 m_fMaxFOV = 120.0f;

    float32 GetTargetFOV() const;
};
