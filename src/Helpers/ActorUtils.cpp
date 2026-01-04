#include "ActorUtils.h"
#include "Math.h"

#include <Glacier/ZSpatialEntity.h>

std::vector<ZActor*> Utils::GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified)
{
    std::vector<ZActor*> s_aActors;

    if (!Globals::NextActorId || *Globals::NextActorId <= 0)
    {
        return s_aActors;
    }

    for (uint16 i = 0; i < *Globals::NextActorId; i++)
    {
        auto s_pActor = Globals::ActorManager->m_aActiveActors[i].m_pInterfaceRef;
        if (!s_pActor)
        {
            continue;
        }

        if ((!p_bIncludeDead && s_pActor->IsDead()) ||
            (!p_bIncludePacified && s_pActor->IsPacified()))
        {
            continue;
        }

        s_aActors.push_back(s_pActor);
    }

    return s_aActors;
}

ZActor* Utils::GetRandomActor(const bool p_bRequireAlive)
{
    auto s_aActors = GetActors(!p_bRequireAlive, !p_bRequireAlive);
    if (s_aActors.empty())
    {
        return nullptr;
    }

    return Math::SelectRandomElement(s_aActors);
}

std::vector<std::pair<ZEntityRef, float32>> Utils::GetNearbyActors(
    const float4& p_vPosition,
    const int p_nMaxCount,
    const float32 p_fRadius,
    const bool p_bIncludeDead,
    const bool p_bIncludePacified)
{
    if (p_nMaxCount == 0)
    {
        return {};
	}

    ZEntityRef s_rClosestActor = {};
    float32 s_fClosestDistance = FLT_MAX;
	std::vector<std::pair<ZEntityRef, float32>> s_aNearbyActors;
    for (auto* s_pActor : Utils::GetActors(p_bIncludeDead, p_bIncludePacified))
    {
        ZEntityRef s_rActor;
        s_pActor->GetID(s_rActor);
        if (!s_rActor)
        {
            continue;
        }

        const auto* s_pActorSpatial = s_rActor.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_ActorWM = s_pActorSpatial->GetWorldMatrix();
        const auto s_fDistance = float4::Distance(p_vPosition, s_ActorWM.Trans);

        // skip if out of range
        if (s_fDistance > p_fRadius)
        {
            continue;
        }

		// track closest actor for n==1 case
        if (s_fClosestDistance > s_fDistance)
        {
            s_fClosestDistance = s_fDistance;
            s_rClosestActor = s_rActor;
		}

		// only populate list if we need more than 1
		// we use a shortcut here to avoid unnecessary calculations
        if (p_nMaxCount > 1)
        {
            s_aNearbyActors.emplace_back(s_rActor, s_fDistance);
		}
    }

    if (p_nMaxCount == 1)
    {
		s_aNearbyActors.emplace_back(s_rClosestActor, s_fClosestDistance);
		return s_aNearbyActors;
    }

    // for n > 1, sort by distance and return the closest n
    std::sort(
        s_aNearbyActors.begin(),
        s_aNearbyActors.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    );

    if (s_aNearbyActors.size() > p_nMaxCount)
    {
		s_aNearbyActors.resize(p_nMaxCount);
    }

	return s_aNearbyActors;
}
