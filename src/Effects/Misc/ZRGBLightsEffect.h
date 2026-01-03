#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/SColorRGB.h>

#include "Helpers/EntityUtils.h"

#include <vector>

class ZRGBLightsEffect : public IChaosEffect
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
		return "Party Lights";
	}

private:
	struct SLightEntityInfo
	{
		ZEntityRef m_rLightEntity;
		bool m_bOriginalVisible;
		float32 m_fOriginalBrightness;
		SColorRGB m_vOriginalColor;

		SLightEntityInfo(ZEntityRef p_rLightEntity) :
			m_rLightEntity(p_rLightEntity),
			m_bOriginalVisible(false),
			m_fOriginalBrightness(0.0f),
			m_vOriginalColor({ 0.0f, 0.0f, 0.0f })
		{
			if (m_rLightEntity)
			{
				m_bOriginalVisible = Utils::GetProperty<bool>(m_rLightEntity, "m_bVisible").value_or(false);
				m_fOriginalBrightness = Utils::GetProperty<float32>(m_rLightEntity, "m_fDiffusePower").value_or(0.0f);
				m_vOriginalColor = Utils::GetProperty<SColorRGB>(m_rLightEntity, "m_diffuseColor").value_or(SColorRGB{ 1.0f, 1.0f, 1.0f });
			}
		}

		inline void Apply(const bool p_bVisible, const float32 p_fNewBrightness, const SColorRGB& p_NewColor)
		{
			if (m_rLightEntity)
			{
				Utils::SetProperty<bool>(m_rLightEntity, "m_bVisible", p_bVisible);
				Utils::SetProperty<float32>(m_rLightEntity, "m_fDiffusePower", p_fNewBrightness);
				Utils::SetProperty<SColorRGB>(m_rLightEntity, "m_diffuseColor", p_NewColor);
			}
		}

		inline void Restore()
		{
			Apply(m_bOriginalVisible, m_fOriginalBrightness, m_vOriginalColor);
		}
	};
	
	std::vector<SLightEntityInfo> m_aLights;

	bool m_bActive = false;
	float32 m_fTimeElapsed = 0.0f;
	float32 m_fTimeToNextChange = 0.0f;
};
