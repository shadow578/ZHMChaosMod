#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

class ZBouncyMinimapEffect : public IChaosEffect
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
        return "Bouncy Minimap";
    }

  private:
    SVector2 m_vBoundsMin = SVector2(300.f, -2315.f);
    SVector2 m_vBoundsMax = SVector2(5035.f, 90.f);
    float32 m_fVelocityMultiplier = 200.f;

  private:
    bool m_bActive = false;
    ZEntityRef m_rMinimapContainer;

    SVector2 m_vPosition = SVector2(0.f, 0.f);
    SVector2 m_vVelocity = SVector2(0.f, 0.f);
};
