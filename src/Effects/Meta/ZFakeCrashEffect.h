#pragma once

#include "IChaosEffect.h"

class ZFakeCrashEffect : public IChaosEffect
{
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) override
    {
        return p_bVoting ? "???" : "Fake Crash";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }

private:

    void SuspendAllThreads();
};