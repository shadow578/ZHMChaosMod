#pragma once

#include "IChaosEffect.h"

#include "Glacier/ZMath.h"

class ZTeleportEffect : public IChaosEffect
{
    void OnDrawDebugUI() override;
    void Start() override;

    std::string GetDisplayName() override
    {
        return "Teleport Player";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }

private:
    enum class ETeleportDirection
    {
        Up,
        Forward,
        NUMBER_OF_DIRECTIONS
    };

    void DoTeleport(const ETeleportDirection p_eDirection);
};
