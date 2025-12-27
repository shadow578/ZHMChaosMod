#pragma once

#include "Effects/Base/ZCameraFOVEffectBase.h"

class ZCameraZoomFOVEffect : public ZCameraFOVEffectBase
{
public:
    ZCameraZoomFOVEffect() : ZCameraFOVEffectBase(10.0f)
    {
    }

    std::string GetDisplayName() override
    {
        return "Binoculars";
    }

    EDuration GetDuration() override
    {
        return EDuration::Short; // painful!
    }
};

class ZCameraWideFOVEffect : public ZCameraFOVEffectBase
{
public:
    ZCameraWideFOVEffect() : ZCameraFOVEffectBase(120.0f)
    {
    }

    std::string GetDisplayName() override
    {
        return "Quake PRO";
    }

    EDuration GetDuration() override
    {
        return EDuration::Full;
    }
};

