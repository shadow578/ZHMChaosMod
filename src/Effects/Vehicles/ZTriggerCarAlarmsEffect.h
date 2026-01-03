#pragma once
#include "IChaosEffect.h"

#include <vector>

class ZTriggerCarAlarmsEffect : public IChaosEffect
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
		return "Oops! All Car Alarms";
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return IChaosEffect::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnEnterScene |
			ELifecycleMethodFlag::OnClearScene;
	}

private:
	std::vector<ZEntityRef> m_aVehicleCores;

	bool m_bActive = false;
};
