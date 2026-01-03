#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include <vector>

class ZSwapTargetsEffect : public IChaosEffect
{
public:
	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start() override;

	void OnDrawDebugUI() override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Swap Targets";
	}

	EDuration GetDuration() const override
	{
		return EDuration::OneShot;
	}

private:
	std::vector<TEntityRef<ZSpatialEntity>> m_aTargetSpatials;
};
