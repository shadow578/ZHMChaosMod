#pragma once
#include "Effects/Base/ZSingleTimeEffectBase.h"

#include <vector>

class ZExplodeAllVehiclesEffect : public ZSingleTimeEffectBase
{
public:
	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Detroit Simulator";
	}

	EDuration GetDuration() const override
	{
		return EDuration::OneShot;
	}

private:
	std::vector<ZEntityRef> m_aFuelTanks;
};
