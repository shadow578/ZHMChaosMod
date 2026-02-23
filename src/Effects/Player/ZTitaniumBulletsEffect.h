#pragma once
#include "Effects/Base/ZRepositoryPatchEffectBase.h"

class ZTitaniumBulletsEffect : public ZRepositoryPatchEffectBase
{
public:
	void Start() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Titanium Bullets"; 
	}
};
