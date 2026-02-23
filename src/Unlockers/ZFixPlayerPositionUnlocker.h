#pragma once
#include "IUnlocker.h"

class ZFixPlayerPositionUnlocker : public IUnlocker
{
public:
    void Start() override;

    std::string GetDisplayName() const override
    {
        return "Fix Player Position";
    }

    std::string GetDescription() const override
    {
        return "Restores the player position to one of the maps starting locations.";
    }
};
