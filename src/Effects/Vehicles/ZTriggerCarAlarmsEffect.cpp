#include "ZTriggerCarAlarmsEffect.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

void ZTriggerCarAlarmsEffect::OnEnterScene()
{
	m_aVehicleCores = Utils::ZEntityFinder()
		.BlueprintResource<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_core.entitytemplate].pc_entityblueprint">()
		.Find();
}

void ZTriggerCarAlarmsEffect::OnClearScene()
{
	m_aVehicleCores.clear();
}

bool ZTriggerCarAlarmsEffect::Available() const
{
	return IChaosEffect::Available() && !m_aVehicleCores.empty();
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
