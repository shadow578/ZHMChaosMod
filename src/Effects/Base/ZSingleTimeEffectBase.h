#pragma once
#include "IChaosEffect.h"

class ZSingleTimeEffectBase : public virtual IChaosEffect
{
public:
	void OnEnterScene() override
	{
		m_bTriggered = false;
	}

	void Start() override
	{
		m_bTriggered = true;
	}

	bool Available() const override
	{
		return IChaosEffect::Available() && !m_bTriggered;
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return IChaosEffect::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnEnterScene;
	}

private:
	bool m_bTriggered = false;
};