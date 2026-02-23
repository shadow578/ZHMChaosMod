#include "ZExplodeAllVehiclesEffect.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

void ZExplodeAllVehiclesEffect::OnEnterScene()
{
	ZSingleTimeEffectBase::OnEnterScene();

	m_aFuelTanks = Utils::ZEntityFinder()
		.BlueprintResource<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_fueltank_a.entitytemplate].pc_entityblueprint">()
		.Find();
}

void ZExplodeAllVehiclesEffect::OnClearScene()
{
	m_aFuelTanks.clear();
}

bool ZExplodeAllVehiclesEffect::Available() const
{
	return ZSingleTimeEffectBase::Available() && !m_aFuelTanks.empty();
}

void ZExplodeAllVehiclesEffect::Start()
{
	ZSingleTimeEffectBase::Start();

	for (const auto& s_rFuelTank : m_aFuelTanks)
	{
		s_rFuelTank.SignalInputPin("Destroy");
	}
}

REGISTER_CHAOS_EFFECT(ZExplodeAllVehiclesEffect)
