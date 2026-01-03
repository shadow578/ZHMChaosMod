#include "ZSwapTargetsEffect.h"

#include <Glacier/ZActor.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"

#include <algorithm>
#include <random>

#define TAG "[ZSwapTargetsEffect] "

void ZSwapTargetsEffect::OnEnterScene()
{
	m_aTargetSpatials.clear();

	const Utils::EntityFinder::SSearchParams s_Query{
		// probably ok...
		.m_sEntityName = "LD_MapTracker_NPCActor"
	};
	const auto& s_aMapTrackers = Utils::EntityFinder::FindEntities(s_Query);

	for (const auto& s_rMapTracker : s_aMapTrackers)
	{
		// check maker is on "targets" layer
		const auto s_eLayer = Utils::GetProperty<UIMapLayer_EUIMapLayerID>(s_rMapTracker, "m_eLayer");
		if (s_eLayer != UIMapLayer_EUIMapLayerID::eUIMLI_TARGET)
		{
			continue;
		}

		// actor is logical parent
		auto s_rActor = s_rMapTracker.GetLogicalParent();
		if (!s_rActor)
		{
			continue;
		}

		auto s_rSpatial = TEntityRef<ZSpatialEntity>(s_rActor);
		if (!s_rSpatial)
		{
			continue;
		}

		// ensure not duplicates
		bool s_bShouldAdd = true;
		for (const auto& s_rExisting : m_aTargetSpatials)
		{
			if (s_rExisting.m_ref == s_rSpatial.m_ref)
			{
				s_bShouldAdd = false;
			}
		}

		if (s_bShouldAdd)
		{
			m_aTargetSpatials.push_back(s_rSpatial);
		}
	}

	Logger::Debug(TAG "located {} targets", m_aTargetSpatials.size());
}

void ZSwapTargetsEffect::OnClearScene()
{
	m_aTargetSpatials.clear();
}


bool ZSwapTargetsEffect::Available() const
{
	return IChaosEffect::Available() && 
		m_aTargetSpatials.size() >= 2;
}

void ZSwapTargetsEffect::Start()
{
	// gather transforms
	std::vector<SMatrix> s_aTransforms;
	for (auto& s_rSpatial : m_aTargetSpatials)
	{
		s_aTransforms.push_back(s_rSpatial.m_pInterfaceRef->GetWorldMatrix());
	}

	// shuffle
	static std::random_device rd;
	static std::mt19937 rng(rd());
	std::shuffle(s_aTransforms.begin(), s_aTransforms.end(), rng);

	// reapply transforms in new order
	size_t s_nIndex = 0;
	for (auto& s_rSpatial : m_aTargetSpatials)
	{
		const auto& s_mTransform = s_aTransforms[s_nIndex];
		s_rSpatial.m_pInterfaceRef->SetWorldMatrix(s_mTransform);

		s_nIndex = (s_nIndex + 1) % s_aTransforms.size();
	}
}

void ZSwapTargetsEffect::OnDrawDebugUI()
{
	for (auto& s_rTarget : m_aTargetSpatials)
	{
		const auto* s_pActor = s_rTarget.m_ref.QueryInterface<ZActor>();
		if (ImGui::Button(s_pActor->m_sActorName.c_str()))
		{
			Utils::TeleportPlayerTo(s_rTarget.m_pInterfaceRef->GetWorldMatrix());
		}
	}
}

REGISTER_CHAOS_EFFECT(ZSwapTargetsEffect)
