#pragma once

#include "IChaosEffect.h"

#include "Glacier/ZMath.h"

class ZSwapPlayerWithActorEffect : public IChaosEffect
{
    void OnDrawDebugUI() override;

    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Swap Player with Random Actor";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

private:
    SMatrix m_PreviousPlayerPosition;
};