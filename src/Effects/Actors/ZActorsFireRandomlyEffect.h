#pragma once
#include "Effects/Base/ZActorWeaponCustomControlEffectBase.h"

class ZActorsFireRandomlyEffect : public ZActorWeaponCustomControlEffectBase
{
public:
	void Start() override;
	void Stop() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "This Is America";
	}

private:
	void SetAllActorsShootingContinuous(const bool p_bShooting);
};
