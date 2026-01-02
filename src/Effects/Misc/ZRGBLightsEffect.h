#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/SColorRGB.h>

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
			m_rLightEntity(p_rLightEntity)
		{
			if (m_rLightEntity)
			{
				m_bOriginalVisible = m_rLightEntity.GetProperty<bool>("m_bVisible").Get();
				m_fOriginalBrightness = m_rLightEntity.GetProperty<float32>("m_fDiffusePower").Get();
				m_vOriginalColor = m_rLightEntity.GetProperty<SColorRGB>("m_diffuseColor").Get();
			}
		}

		inline void Apply(const bool p_bVisible, const float32 p_fNewBrightness, const SColorRGB& p_NewColor)
		{
			if (m_rLightEntity)
			{
				m_rLightEntity.SetProperty("m_bVisible", p_bVisible);
				m_rLightEntity.SetProperty("m_fDiffusePower", p_fNewBrightness);
				m_rLightEntity.SetProperty("m_diffuseColor", p_NewColor);
			}
		}

		inline void Restore()
		{
			Apply(m_bOriginalVisible, m_fOriginalBrightness, m_vOriginalColor);
		}
	};
	
	std::vector<SLightEntityInfo> m_aLights;
	bool m_bLightsLoaded = false;

	bool m_bActive = false;
	float32 m_fTimeElapsed = 0.0f;
	float32 m_fTimeToNextChange = 0.0f;
};
