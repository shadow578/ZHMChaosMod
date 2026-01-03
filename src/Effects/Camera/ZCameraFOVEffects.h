#pragma once

#include "Effects/Base/ZCameraFOVEffectBase.h"

class ZCameraZoomFOVEffect : public ZCameraFOVEffectBase
{
public:
    ZCameraZoomFOVEffect() : ZCameraFOVEffectBase(10.0f)
    {
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Binoculars";
    }

    EDuration GetDuration() const override
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

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Quake PRO";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Full;
    }
};
