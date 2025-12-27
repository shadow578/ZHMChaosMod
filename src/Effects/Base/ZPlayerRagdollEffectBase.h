#pragma once

#include "IChaosEffect.h"

class ZPlayerRagdollEffectBase : public virtual IChaosEffect
{
public:
    ZPlayerRagdollEffectBase(const bool p_bPowered) : m_bPowered(p_bPowered) {}

    void Start() override;
    void Stop() override;

    bool Available() override;
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

private:
    const bool m_bPowered;
};
