#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

class ZShakyReticleEffect : public IChaosEffect
{
  public:
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Shaky Hands";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

  private:
    float32 m_fShakeAmplitude = 12.f;
    float32 m_fMaxDeflection = 150.f;

  private:
    bool m_bActive = false;
    ZEntityRef m_rHUDReticle;

    SVector2 m_vPosition = SVector2(0.f, 0.f);
};
