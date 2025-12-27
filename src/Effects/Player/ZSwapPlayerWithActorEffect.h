#pragma once

#include "IChaosEffect.h"

#include "Glacier/ZMath.h"

class ZSwapPlayerWithActorEffect : public IChaosEffect
{
    void OnDrawDebugUI() override;

    void Start() override;

    std::string GetDisplayName() override
    {
        return "Swap Player with Random Actor";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }

private:
    SMatrix m_PreviousPlayerPosition;
};