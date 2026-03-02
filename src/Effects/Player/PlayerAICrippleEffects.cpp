#include "PlayerAICrippleEffects.h"

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

void ZPlayerHiddenEffect::Start()
{
    GetAICripple().SetHeroHidden();
}

void ZPlayerHiddenEffect::Stop()
{
    GetAICripple().SetHeroVisible();
}

REGISTER_CHAOS_EFFECT(ZPlayerHiddenEffect);
