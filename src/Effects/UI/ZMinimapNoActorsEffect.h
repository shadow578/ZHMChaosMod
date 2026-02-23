#pragma once
#include "IChaosEffect.h"

class ZMinimapNoActorsEffect : public IChaosEffect
{
public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "No Actors on Minimap";
    }

private:
    void SetMinimapType(const EMapType p_eMapType);
};
