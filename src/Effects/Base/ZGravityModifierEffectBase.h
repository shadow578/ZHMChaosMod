#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZMath.h>

class ZGravityModifierEffectBase : public IChaosEffect
{
public:
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

protected:
    void RestoreDefaultGravity();
    void SetGravity(const SVector3 p_vGravity);
};
