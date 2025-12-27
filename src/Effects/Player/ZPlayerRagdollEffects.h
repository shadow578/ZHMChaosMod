#pragma once
#include "Effects/Base/ZPlayerRagdollEffectBase.h"

#include "Helpers/ZVelocityTracker.h"

class ZPlayerSimpleRagdollEffect : public ZPlayerRagdollEffectBase
{
public:
    ZPlayerSimpleRagdollEffect() : ZPlayerRagdollEffectBase(false) {}

    std::string GetDisplayName() const  override
    {
        return "Ragdoll Player";
    }

    EDuration GetDuration() const  override
    {
        return EDuration::Short; // boring if too long
    }
};

class ZPlayerPoweredRagdollEffect : public ZPlayerRagdollEffectBase
{
public:
    ZPlayerPoweredRagdollEffect() : ZPlayerRagdollEffectBase(true) {}

    std::string GetDisplayName() const override
    {
        return "Floppy 47";
    }
};

class ZPlayerRagdollWithImpulseEffect : public ZPlayerRagdollEffectBase
{
public:
    ZPlayerRagdollWithImpulseEffect() : ZPlayerRagdollEffectBase(false) {}

    void Start() override;
    void Stop() override;
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName() const override
    {
        return "Super Stunt";
    }

    EDuration GetDuration() const override
    {
        // while there's a persistent effect, it's cancelled after the "stunt"
        return EDuration::OneShot;
    }

private:
    bool m_bActive = false;
    ZVelocityTracker m_VelocityTracker;
};
