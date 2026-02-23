#pragma once
#include "Effects/Base/Companion/ZExplosionEffectBase.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZActor.h>

#include <vector>

class ZExplodeNearbyActorsEffect : public ZExplosionEffectBase
{
public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Obliterate All Nearby Actors";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }
};