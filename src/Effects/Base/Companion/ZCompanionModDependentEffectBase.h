#pragma once
#include "IChaosEffect.h"
#include "Helpers/CompanionMod.h"

class ZCompanionModDependentEffectBase : public virtual IChaosEffect
{
public:
	ZCompanionModDependentEffectBase(const CompanionModUtil::SVersion p_MinimumVersion = CompanionModUtil::SVersion(0, 0, 0))
		: m_MinimumVersion(p_MinimumVersion)
	{
	}

	bool Available() const override;

private:
	CompanionModUtil::SVersion m_MinimumVersion;
};
