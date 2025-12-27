#include "ZSimpleGravityModifierEffect.h"

#include "EffectRegistry.h"

void ZSimpleGravityModifierEffect::Start()
{
    SetGravity(m_vGravity);
}

void ZSimpleGravityModifierEffect::Stop()
{
    RestoreDefaultGravity();
}

REGISTER_CHAOS_EFFECT_PARAM(Low, ZSimpleGravityModifierEffect, "Moon", "Moon Gravity", SVector3(0.f, 0.f, -1.625f));
REGISTER_CHAOS_EFFECT_PARAM(High, ZSimpleGravityModifierEffect, "High", "Jupiter Gravity", SVector3(0.f, 0.f, -24.79f));
REGISTER_CHAOS_EFFECT_PARAM(Inverse, ZSimpleGravityModifierEffect, "Inverse", "Inverse Gravity", SVector3(0.f, 0.f, 9.81f));
REGISTER_CHAOS_EFFECT_PARAM(Sideways, ZSimpleGravityModifierEffect, "Sideways", "Sideways Gravity", SVector3(-5.f, -5.f, 0.f));
