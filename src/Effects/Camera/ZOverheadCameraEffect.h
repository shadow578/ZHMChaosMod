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

    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "GTA1 Mode";
    }

  private:
    float32 GetMaxOverheadZAt(const SMatrix& p_Position, const float32 p_fMax);
};
