#include "ZRGBLightsEffect.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZRGBLightsEffect] "

void ZRGBLightsEffect::OnEnterScene()
{
	m_aLights.clear();

	const auto s_aLightEntities = Utils::ZEntityFinder()
		.EntityType("ZLightEntity")
		.Find();

	for (auto& s_rLightEntity : s_aLightEntities)
	{
		// exclude environmental and area lights
		const auto s_eType = Utils::GetProperty<ILightEntity_ELightType>(s_rLightEntity, "m_eLightType");
		if (s_eType == ILightEntity_ELightType::LT_DIRECTIONAL ||
			s_eType == ILightEntity_ELightType::LT_OMNI ||
			s_eType == ILightEntity_ELightType::LT_SPOT || 
			s_eType == ILightEntity_ELightType::LT_SQUARESPOT)
		{
			m_aLights.push_back(SLightEntityInfo(s_rLightEntity));
		}
	}

	Logger::Debug(TAG "Found {} light entities!", m_aLights.size());
}

void ZRGBLightsEffect::OnClearScene()
{
	m_aLights.clear();
}

bool ZRGBLightsEffect::Available() const
{
	return IChaosEffect::Available() && !m_aLights.empty();
}

void ZRGBLightsEffect::Start()
{
	m_bActive = true;
	m_fTimeElapsed = 0.0f;
	m_fTimeToNextChange = 0.0f;
}

void ZRGBLightsEffect::Stop()
{
	m_bActive = false;

	for (auto& s_LightInfo : m_aLights)
	{
		s_LightInfo.Restore();
	}
}

void ZRGBLightsEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	m_fTimeElapsed += p_fDeltaTime;
	m_fTimeToNextChange -= p_fDeltaTime;
	if (m_fTimeToNextChange > 0.0f)
	{
		return;
	}

	// set global color with random brightness per light
	const SColorRGB s_Color = Math::GetRainbowColor(m_fTimeElapsed, 0.0f);
	for (auto& s_LightInfo : m_aLights)
	{
		const float32 s_Brightness = Math::GetRandomNumber(400.0f, 1500.0f);
		s_LightInfo.Apply(true, s_Brightness, s_Color);
	}

	m_fTimeToNextChange = Math::GetRandomNumber(0.1f, 0.3f);
}

REGISTER_CHAOS_EFFECT(ZRGBLightsEffect)
