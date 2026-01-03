#include "ZFireworksEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include <Helpers/EntityUtils.h>

static ZTemplateEntitySpawner<"[assembly:/_pro/effects/templates/fire/fx_fireworks.template?/fx_fireworks_launchpad.entitytemplate].pc_entitytype"> g_FireworksProp;

void ZFireworksEffect::LoadResources()
{
	m_pFireworksSpawner = g_FireworksProp.CreateSession();
}

void ZFireworksEffect::OnClearScene()
{
	m_pFireworksSpawner = nullptr;
	m_rFireworksEntity = {};
}

bool ZFireworksEffect::Available() const
{
	return IChaosEffect::Available() &&
		m_pFireworksSpawner && m_pFireworksSpawner->IsAvailable();
}

void ZFireworksEffect::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("Prop: {}", m_pFireworksSpawner->ToString()).c_str());
}

void ZFireworksEffect::Start()
{
	auto s_rFireworksBarge = m_pFireworksSpawner->Spawn();
	if (!s_rFireworksBarge)
	{
		return;
	}

	// aquire refs to sub-entities
	ZEntityRef s_rFireworksFXEntity,
		s_rSwirlMachine1, s_rSwirlMachine2,
		s_rTimer0, s_rTimer1, s_rTimer2, s_rTimer3;
	if (auto* s_pBlueprint = s_rFireworksBarge.GetBlueprintFactory())
	{
		// Fireworks
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x3fa6ea87b6e7bfec); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rFireworksFXEntity = ZEntityRef(s_pEntity);
			}
		}

		// firework_swirlmachine_a
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x0d26e9029d9cdb1a); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rSwirlMachine1 = ZEntityRef(s_pEntity);
			}
		}

		// firework_swirlmachine_a01
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x03c72250d57f0356); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rSwirlMachine2 = ZEntityRef(s_pEntity);
			}
		}

		// TimerSimple
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xbc53160f645abffc); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rTimer0 = ZEntityRef(s_pEntity);
			}
		}

		// TimerSimple01
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x36a50d14be2f2f88); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rTimer1 = ZEntityRef(s_pEntity);
			}
		}

		// TimerSimple02
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xdd9b974ddf3feda5); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rTimer2 = ZEntityRef(s_pEntity);
			}
		}

		// TimerSimple03
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x3ac42cc89a95ba81); s_nIdx != -1)
		{
			if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rFireworksBarge.m_pEntity, s_nIdx))
			{
				s_rTimer3 = ZEntityRef(s_pEntity);
			}
		}
	}
	if (!s_rFireworksFXEntity)
	{
		// only FX entity is essential, the rest is added bonuses
		return;
	}

	// detach FX entity from barge and move barge out of sight
	Utils::SetProperty<ZEntityRef>(s_rFireworksFXEntity, "m_eidParent", ZEntityRef{});
	Utils::SetProperty<bool>(s_rFireworksBarge, "m_bVisible", false);
	if (auto* s_pBargeSpatial = s_rFireworksBarge.QueryInterface<ZSpatialEntity>())
	{
		SMatrix s_mWorldAway;
		s_mWorldAway.Trans.z -= 1000;
		s_pBargeSpatial->SetWorldMatrix(s_mWorldAway);
	}

	// hide swirlmachines (yes, IOI calls them that...)
	// not deparenting to also hide trails
	Utils::SetProperty<bool>(s_rSwirlMachine1, "m_bVisible", false);
	Utils::SetProperty<bool>(s_rSwirlMachine2, "m_bVisible", false);

	// speed up timers to make more fireworks appear
	const std::string c_sDelayName = "Delay time (ms)";
	if (auto s_nDelay = Utils::GetProperty<int32>(s_rTimer0, c_sDelayName))
	{
		Utils::SetProperty<int32>(s_rTimer0, c_sDelayName, *s_nDelay / 2);
	}
	if (auto s_nDelay = Utils::GetProperty<int32>(s_rTimer1, c_sDelayName))
	{
		Utils::SetProperty<int32>(s_rTimer1, c_sDelayName, *s_nDelay / 2);
	}
	if (auto s_nDelay = Utils::GetProperty<int32>(s_rTimer2, c_sDelayName))
	{
		Utils::SetProperty<int32>(s_rTimer2, c_sDelayName, *s_nDelay / 2);
	}
	if (auto s_nDelay = Utils::GetProperty<int32>(s_rTimer3, c_sDelayName))
	{
		Utils::SetProperty<int32>(s_rTimer3, c_sDelayName, *s_nDelay / 2);
	}

	// move FX to player
	if (auto s_rFireworksFXSpatial = TEntityRef<ZSpatialEntity>(s_rFireworksFXEntity))
	{
		if (auto s_rPlayer = SDK()->GetLocalPlayer())
		{
			if (auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_ref))
			{
				s_rFireworksFXSpatial.m_pInterfaceRef->SetWorldMatrix(s_rPlayerSpatial.m_pInterfaceRef->GetWorldMatrix());
			}
		}
	}

	// go!
	s_rFireworksFXEntity.SignalInputPin("Activate");
	m_rFireworksEntity = s_rFireworksFXEntity;
}

void ZFireworksEffect::Stop()
{
	if (m_rFireworksEntity)
	{
		m_rFireworksEntity.SignalInputPin("Deactivate");
		m_rFireworksEntity = {};
	}
}

REGISTER_CHAOS_EFFECT(ZFireworksEffect);
