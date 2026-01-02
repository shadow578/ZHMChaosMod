#pragma once
#include "Base/ZRenderPostfilterEffectBase.h"

class ZHeavyFogEffect : public ZRenderPostfilterEffectBase
{
public:
	ZHeavyFogEffect()
	{
	}

	void Start() override;
	void Stop() override;

	std::string GetDisplayName() const override
	{
		return "Silent Hill";
	}
};
