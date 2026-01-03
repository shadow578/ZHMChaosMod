#include "ZReturnToSpawnEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZReturnToSpawnEffect] "

void ZReturnToSpawnEffect::OnEnterScene()
{
    m_aSpawnPoints.clear();
	m_bSpawnPointsLoaded = true;

    const Utils::EntityFinder::SSearchParams s_Query{
        .m_sEntityType = "ZHeroSpawn"
    };
    const auto s_aStartingLocations = Utils::EntityFinder::FindEntities(s_Query);
    if (s_aStartingLocations.empty())
    {
        return;
    }

    for (const auto& s_StartingLocation : s_aStartingLocations)
    {
        // attempt to get position property first
        auto s_rPosition = Utils::GetProperty<TEntityRef<ZSpatialEntity>>(s_StartingLocation, "m_rPosition").value_or({});
        if (!s_rPosition)
        {
            // if that fails, fall back to the spatial entity of the location itself
			s_rPosition = TEntityRef<ZSpatialEntity>(s_StartingLocation);
            if (!s_rPosition)
            {
                // give up
                continue;
			}
        }

        m_aSpawnPoints.push_back(s_rPosition.m_pInterfaceRef->GetWorldMatrix());
    }

	Logger::Debug(TAG "Found {} spawn points.", m_aSpawnPoints.size());
}

void ZReturnToSpawnEffect::OnClearScene()
{
    m_aSpawnPoints.clear();
	m_bSpawnPointsLoaded = false;
}

bool ZReturnToSpawnEffect::Available() const
{
    return IChaosEffect::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
        (!m_bSpawnPointsLoaded || !m_aSpawnPoints.empty());
}

void ZReturnToSpawnEffect::Start()
{
	const auto s_SpawnPoint = Math::SelectRandomElement(m_aSpawnPoints);
	Utils::TeleportPlayerTo(s_SpawnPoint);
}

void ZReturnToSpawnEffect::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("# Spawn Points: {}", m_aSpawnPoints.size()).c_str());

    int i = 0;
    for (const auto& s_SpawnPoint : m_aSpawnPoints)
    {
        if (ImGui::Button(fmt::format("[{}]", i).c_str()))
        {
            Utils::TeleportPlayerTo(s_SpawnPoint);
		}

        i++;
    }
}

REGISTER_CHAOS_EFFECT(ZReturnToSpawnEffect)
