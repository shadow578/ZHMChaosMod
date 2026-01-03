#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZMath.h>

#include <vector>

class ZReturnToSpawnEffect : public IChaosEffect
{
public:
	void OnEnterScene() override;
	void OnClearScene() override;
	void OnDrawDebugUI() override;
	bool Available() const override;

	void Start() override;

	std::string GetDisplayName() const override
	{
		return "Return to Spawn";
	}

	EDuration GetDuration() const override
	{
		return EDuration::OneShot;
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return IChaosEffect::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnEnterScene |
			ELifecycleMethodFlag::OnClearScene;
	}

private:
	std::vector<SMatrix> m_aSpawnPoints;
};
