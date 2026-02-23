#pragma once
#include "IUnlocker.h"

class ZFixCameraUnlocker : public IUnlocker
{
public:
    void Start() override;

    std::string GetDisplayName() const override
    {
        return "Fix Player Camera";
    }

    std::string GetDescription() const override
    {
        return "Restores the default player camera, in case it gets stuck due to an effect.";
    }
};
