#include "ZLockDoorsEffect.h"

#include "EffectRegistry.h"

void ZLockDoorsEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	auto& s_aDoors = GetDoors();
	for (auto& s_rDoor : s_aDoors)
	{
		s_rDoor.CloseDoor();
		s_rDoor.LockDoor();
	}
}

REGISTER_CHAOS_EFFECT(ZLockDoorsEffect);
