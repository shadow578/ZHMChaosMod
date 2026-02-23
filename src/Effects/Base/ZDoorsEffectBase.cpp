#include "ZDoorsEffectBase.h"

#include "Helpers/EntityUtils.h"

#define TAG "[ZDoorsEffectBase] "

void ZDoorsEffectBase::OnEnterScene()
{
	m_aDoors.clear();

	const auto& s_aSingleDoors = Utils::ZEntityFinder()
		.BlueprintResource<"[modules:/zhm5singledoor2.class].pc_entityblueprint">()
		.Find();
	const auto& s_aDoubleDoors = Utils::ZEntityFinder()
		.BlueprintResource<"[modules:/zhm5doubledoor2.class].pc_entityblueprint">()
		.Find();

	for (const auto& s_rDoor : s_aSingleDoors)
	{
		auto s_DoorBinding = SHM5SingleDoor2Binding(s_rDoor);
		if (s_DoorBinding)
		{
			m_aDoors.push_back(s_DoorBinding);
		}
	}

	for (const auto& s_rDoor : s_aDoubleDoors)
	{
		auto s_DoorBinding = SHM5SingleDoor2Binding(s_rDoor);
		if (s_DoorBinding)
		{
			m_aDoors.push_back(s_DoorBinding);
		}
	}

	Logger::Debug(TAG "found {} doors to mess with", m_aDoors.size());
}

void ZDoorsEffectBase::OnClearScene()
{
	m_aDoors.clear();
}

bool ZDoorsEffectBase::Available() const
{
	return IChaosEffect::Available()
		&& m_aDoors.size() >= m_nMinDoors;
}

void ZDoorsEffectBase::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("# Doors: {}", m_aDoors.size()).c_str());
}
