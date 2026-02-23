#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityBindings/SInteractionSubactionEntityBinding.h"

#include <vector>

class ZTriggerInteractionEffect : public IChaosEffect
{
public:
	ZTriggerInteractionEffect(const std::string p_sNameSuffix, const std::string p_sDisplayName, const float32 p_fRadius)
		: m_sNameSuffix(p_sNameSuffix),
		m_sDisplayName(p_sDisplayName),
		m_fRadius(p_fRadius)
	{
	}

	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start();

	std::string GetName() const
	{
		return IChaosEffect::GetName() + "_" + m_sNameSuffix;
	}

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		if (p_bVoting || m_sLastInteractionText.empty())
		{
			return m_sDisplayName;
		}

		return m_sDisplayName + "(" + m_sLastInteractionText + ")";
	}

	EDuration GetDuration() const override
	{
		return EDuration::OneShot;
	}

private:
	const std::string m_sNameSuffix;
	const std::string m_sDisplayName;
	const float32 m_fRadius;
	std::vector<SInteractionSubactionEntityBinding> m_aInteractionEntities;
	std::string m_sLastInteractionText;
};
