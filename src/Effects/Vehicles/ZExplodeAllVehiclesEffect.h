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

	std::string GetDisplayName() const override
	{
		return "Detroit Simulator";
	}

	EDuration GetDuration() const override
	{
		return EDuration::OneShot;
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return ZSingleTimeEffectBase::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnEnterScene |
			ELifecycleMethodFlag::OnClearScene;
	}

private:
	std::vector<ZEntityRef> m_aFuelTanks;
};
