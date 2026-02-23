#pragma once
#include "Effects/Base/ZDoorsEffectBase.h"

class ZOpenCloseDoorsEffect : public ZDoorsEffectBase
{
public:
	ZOpenCloseDoorsEffect() :
		ZDoorsEffectBase(50), // require 50 doors
		m_fInterval(0.75f) {}

	void OnDrawDebugUI() override;

	void Start() override;
	void Stop() override;
	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Spammy Doors";
	}

private:
	bool m_bActive = false;
	bool m_bNextOpenState = true;
	float32 m_fInterval;
	float32 m_fIntervalTimer = 0.0f;
};
