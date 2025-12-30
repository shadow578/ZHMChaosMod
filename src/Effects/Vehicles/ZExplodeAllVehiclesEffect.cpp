#include "ZExplodeAllVehiclesEffect.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

void ZExplodeAllVehiclesEffect::OnEnterScene()
{
	ZSingleTimeEffectBase::OnEnterScene();

	const Utils::EntityFinder::SSearchParams s_Query{
		.m_ridBlueprint = ResId<"[assembly:/_pro/vehicles/templates/vehicle_logic.template?/vehicle_fueltank_a.entitytemplate].pc_entityblueprint">
	};
	m_aFuelTanks = Utils::EntityFinder::FindEntities(s_Query);
	m_bVehiclesLoaded = true;
}

void ZExplodeAllVehiclesEffect::OnClearScene()
{
	m_aFuelTanks.clear();
	m_bVehiclesLoaded = false;
}

bool ZExplodeAllVehiclesEffect::Available() const
{
	return ZSingleTimeEffectBase::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
		(!m_bVehiclesLoaded || !m_aFuelTanks.empty());
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
