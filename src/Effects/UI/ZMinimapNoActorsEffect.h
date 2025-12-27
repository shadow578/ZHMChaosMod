#pragma once
#include "IChaosEffect.h"

class ZMinimapNoActorsEffect : public IChaosEffect
{
public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName() override
    {
        return "No Actors on Minimap";
    }
};
