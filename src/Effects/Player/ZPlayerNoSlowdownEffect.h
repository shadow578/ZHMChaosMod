#pragma once
#include "Effects/Base/Companion/ZExplosionEffectBase.h"
#include "Helpers/ZVelocityTracker.h"

class ZPlayerNoSlowdownEffect : public ZExplosionEffectBase
{
  public:
    void OnDrawUI(const bool p_bHasFocus) override;
    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Speed";
    }

  private:
    enum class EState
    {
        Inactive,        // disabled
        ActivationGrace, // enabled, in grace period for player to prepare
        AtSpeed,         // monitoring speed, currently at target
        BelowSpeedGrace, // monitoring speed, currently below target but in grace period for player to speed up again
        BelowSpeed       // speed below target, go boom after short delay. no recovery from here
    };

    float32 m_fTargetVelocity = 3.5f;
    float32 m_fActivationGraceTime = 5.0f;
    float32 m_fBelowSpeedGraceTime = 2.5f;
    float32 m_fExplosionDelayMin = 0.5f;
    float32 m_fExplosionDelayMax = 1.5f;

    ZVelocityTracker m_VelocityTracker{};
    EState m_eState = EState::Inactive;
    float32 m_fGraceTimeRemaining = 0.0f; // both for ActivationGrace and BelowSpeedGrace, depending on state

    int m_nUIFlash = 0; // for flashing the UI, updated in slow update

    bool IsBelowTargetVelocity() const;
    void Explode();

    static std::string StateToString(const EState p_eState);
};
