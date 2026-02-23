#pragma once
#include "Effects/Base/ZDoorsEffectBase.h"

class ZLockDoorsEffect : public ZDoorsEffectBase
{
public:
	ZLockDoorsEffect() :
		ZDoorsEffectBase(50) // require 50 doors
	{
	}

	void Start() override { m_bActive = true; }
	void Stop() override { m_bActive = false; }
	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "Lock All Doors";
	}

	EDuration GetDuration() const override
	{
		return EDuration::Short;
	}

private:
	bool m_bActive = false;
};
