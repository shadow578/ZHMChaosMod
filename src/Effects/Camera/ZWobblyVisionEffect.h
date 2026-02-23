#pragma once
#include "Base/ZRenderPostfilterEffectBase.h"

class ZWobblyVisionEffect : public ZRenderPostfilterEffectBase
{
public:
	ZWobblyVisionEffect()
	{
	}

	void Start() override;
	void Stop() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Wobbly Vision";
	}

	EDuration GetDuration() const override
	{
		return EDuration::Short; // painful if too long
	}
};
