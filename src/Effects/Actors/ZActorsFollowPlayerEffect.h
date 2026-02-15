#pragma once
#include "Effects/Base/ZCompanionModDependentEffectBase.h"

class ZActorsFollowPlayerEffect : public ZCompanionModDependentEffectBase
{
public:
	ZActorsFollowPlayerEffect()
		: ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 3, 0)) // follow player helper on NPCActor
	{
	}

	void Start() override;
	void Stop() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "You Are Famous";
	}

private:
	void SignalAllActors(const std::string& p_sSignalName);
};
