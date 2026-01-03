#pragma once
#include "Effects/Base/ZExplosionEffectBase.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZActor.h>

#include <vector>

class ZExplodeActorsOnTouchEffect : public ZExplosionEffectBase
{
public:
    void Start() override { m_bEnable = true; }
    void Stop() override { m_bEnable = false; }
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Explody Actors";
    }

private:
    bool m_bEnable = false;

    bool m_bSpawnExplosions = true;
    float32 m_fNearestActorDistance = 0.0f;
};
