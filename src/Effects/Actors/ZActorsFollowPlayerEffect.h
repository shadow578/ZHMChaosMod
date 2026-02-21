#pragma once
#include "Effects/Base/Companion/ZActorFollowPlayerHelperEffectBase.h"

class ZActorsFollowPlayerEffect : public ZActorFollowPlayerHelperEffectBase
{
public:
	void Start() override;
	void Stop() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "You Are Famous";
	}

private:
	void SetActorsFollowPlayer(const bool p_bFollow);
};
