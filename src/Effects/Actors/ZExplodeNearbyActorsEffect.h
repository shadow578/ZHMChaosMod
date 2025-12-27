#pragma once
#include "Effects/Base/ZExplosionEffectBase.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZActor.h>

#include <vector>

class ZExplodeNearbyActorsEffect : public ZExplosionEffectBase
{
public:
    void Start() override;

    std::string GetDisplayName() override
    {
        return "Obliterate All Nearby Actors";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }
};