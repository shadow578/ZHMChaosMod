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

	std::string GetDisplayName() const override
	{
		return "Wobbly Vision";
	}

	EDuration GetDuration() const override
	{
		return EDuration::Short; // painful if too long
	}
};
