#pragma once
#include "Base/ZRenderPostfilterEffectBase.h"

class ZBlurryVisionEffect : public ZRenderPostfilterEffectBase
{
public:
	ZBlurryVisionEffect()
	{
	}

	void Start() override;
	void Stop() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Where are my glasses?";
	}

	EDuration GetDuration() const override
	{
		return EDuration::Short; // painful if too long
	}
};
