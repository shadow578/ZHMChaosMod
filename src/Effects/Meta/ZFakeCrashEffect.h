#pragma once

#include "IChaosEffect.h"

class ZFakeCrashEffect : public IChaosEffect
{
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return p_bVoting ? "???" : "Fake Crash";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

        void LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration) override;
    void DrawConfigUI(ZConfigurationAccessor* p_pConfiguration) override;

  private:
    float32 m_fDuration = 10.f; // seconds

    void SuspendAllThreads();
};