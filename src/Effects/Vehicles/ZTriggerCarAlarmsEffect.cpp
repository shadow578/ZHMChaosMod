#include "ZTriggerCarAlarmsEffect.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

void ZTriggerCarAlarmsEffect::OnEnterScene()
{
	const Utils::EntityFinder::SSearchParams s_Query{
		.m_ridBlueprint = ResId<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_core.entitytemplate].pc_entityblueprint">
	};
	m_aVehicleCores = Utils::EntityFinder::FindEntities(s_Query);
	m_bVehiclesLoaded = true;
}

void ZTriggerCarAlarmsEffect::OnClearScene()
{
	m_aVehicleCores.clear();
	m_bVehiclesLoaded = false;
}

bool ZTriggerCarAlarmsEffect::Available() const
{
	return IChaosEffect::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
		(!m_bVehiclesLoaded || !m_aVehicleCores.empty());
}

void ZTriggerCarAlarmsEffect::Start()
{
	m_bActive = true;

	for (const auto& s_rVehicleCore : m_aVehicleCores)
	{
		s_rVehicleCore.SignalInputPin("TriggerAlarm");
	}
}

void ZTriggerCarAlarmsEffect::Stop()
{
	m_bActive = false;

	for (const auto& s_rVehicleCore : m_aVehicleCores)
	{
		s_rVehicleCore.SignalInputPin("TurnOffAlarm");
	}
}

void ZTriggerCarAlarmsEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	// NPCs may turn off the alarms, retrigger constantly while active
	if (static_cast<int>(p_fEffectTimeRemaining) % 2 == 0)
	{
		Start();
	}
}

REGISTER_CHAOS_EFFECT(ZTriggerCarAlarmsEffect)
