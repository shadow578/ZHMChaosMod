#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZMath.h>

/**
 * Reusable base for modifying physics gravity.
 * Use SetGravity() to set a custom gravity, and 
 * RestoreDefaultGravity() to restore default.
 */
class ZGravityModifierEffectBase : public virtual IChaosEffect
{
public:
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

protected:
    void RestoreDefaultGravity();
    void SetGravity(const SVector3 p_vGravity);
};
