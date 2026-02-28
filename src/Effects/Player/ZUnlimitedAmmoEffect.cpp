#include "ZUnlimitedAmmoEffect.h"

#include "EffectRegistry.h"

void ZUnlimitedAmmoEffect::Start()
{
	auto s_rCripple = GetCrippleBox();
	s_rCripple.m_pInterfaceRef->m_bLimitedAmmo = false;

	ActivateCrippleBox();
}

REGISTER_CHAOS_EFFECT(ZUnlimitedAmmoEffect);
