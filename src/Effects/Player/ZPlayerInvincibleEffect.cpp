#include "ZPlayerInvincibleEffect.h"

#include "EffectRegistry.h"

void ZPlayerInvincibleEffect::Start()
{
	GetAICripple().SetHeroInvincible();
}

void ZPlayerInvincibleEffect::Stop()
{
	GetAICripple().SetHeroVulnerable();
}

REGISTER_CHAOS_EFFECT(ZPlayerInvincibleEffect);
