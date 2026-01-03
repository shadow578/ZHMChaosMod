#pragma once
#include "IChaosEffect.h"

class ZNoHUDEffect : public IChaosEffect
{
public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Where's my HUD?!";
    }

private:
    void SetHUDVisibility(const bool p_bVisible);
};
