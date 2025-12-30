#pragma once
#include "IChaosEffect.h"

#include <Glacier/SColorRGB.h>

#include <vector>

class ZRainbowVehiclesEffect : public IChaosEffect
{
public:
	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start() override;
	void Stop() override;

	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

	std::string GetDisplayName() const override
	{
		return "Rainbow Vehicles";
	}

private:
	std::vector<ZEntityRef> m_aVehicles;
	bool m_bVehiclesLoaded = false;

	bool m_bActive = false;
	float32 m_fTimeElapsed = 0.0f;

	static SColorRGB GetRainbowColor(const float32 p_fTime, const float32 p_fOffset);
};
