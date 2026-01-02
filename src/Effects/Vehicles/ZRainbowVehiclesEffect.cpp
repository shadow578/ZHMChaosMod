#include "ZRainbowVehiclesEffect.h"


#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/Math.h"

void ZRainbowVehiclesEffect::OnEnterScene()
{
	const Utils::EntityFinder::SSearchParams s_Query{
		.m_ridBlueprint = ResId<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_core.entitytemplate].pc_entityblueprint">
	};
	const auto& s_aVehicleCores = Utils::EntityFinder::FindEntities(s_Query);

	for (auto& s_rVehicleCore : s_aVehicleCores)
	{
		auto s_rVehicle = s_rVehicleCore.GetOwningEntity();
		if (!s_rVehicle)
		{
			continue;
		}

		const auto s_ColorProp = s_rVehicle.GetProperty<SColorRGB>("VehiclePaintDiffuse");
		if (s_ColorProp.IsEmpty())
		{
			continue;
		}

		m_aVehicles.push_back(s_rVehicle);
	}

	m_bVehiclesLoaded = true;
}

void ZRainbowVehiclesEffect::OnClearScene()
{
	m_aVehicles.clear();
	m_bVehiclesLoaded = false;
}

bool ZRainbowVehiclesEffect::Available() const
{
	return IChaosEffect::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
		(!m_bVehiclesLoaded || !m_aVehicles.empty());
}

void ZRainbowVehiclesEffect::Start()
{
	m_bActive = true;
	m_fTimeElapsed = 0.0f;
}

void ZRainbowVehiclesEffect::Stop()
{
	m_bActive = false;
}

void ZRainbowVehiclesEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	m_fTimeElapsed += p_fDeltaTime;

	float32 s_fOffset = 0.0f;
	for (auto& s_rVehicle : m_aVehicles)
	{
		const SColorRGB s_Color = Math::GetRainbowColor(m_fTimeElapsed, s_fOffset);
		s_rVehicle.SetProperty("VehiclePaintDiffuse", /*SColorRGB*/ s_Color);

		s_fOffset += 0.137f;
	}
}

REGISTER_CHAOS_EFFECT(ZRainbowVehiclesEffect)
