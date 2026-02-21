#include "ActorUtils.h"
#include "Math.h"

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityUtils.h"

std::vector<TEntityRef<ZActor>> Utils::GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified)
{
    std::vector<TEntityRef<ZActor>> s_aActors;

    if (!Globals::ActorManager)
    {
        return s_aActors;
    }

    for (auto& s_rActor : Globals::ActorManager->m_activatedActors)
    {
        if (!s_rActor)
        {
            continue;
        }

        if ((!p_bIncludeDead && s_rActor.m_pInterfaceRef->IsDead()) ||
            (!p_bIncludePacified && s_rActor.m_pInterfaceRef->IsPacified()))
        {
            continue;
        }

        s_aActors.push_back(s_rActor);
    }

    return s_aActors;
}

TEntityRef<ZActor> Utils::GetRandomActor(const bool p_bRequireAlive)
{
    auto s_aActors = GetActors(!p_bRequireAlive, !p_bRequireAlive);
    if (s_aActors.empty())
    {
        return {};
    }

    return Math::SelectRandomElement(s_aActors);
}

std::vector<std::pair<TEntityRef<ZActor>, float32>> Utils::GetNearbyActors(
    const float4 &p_vPosition,
    const int p_nMaxCount,
    const float32 p_fRadius,
    const bool p_bIncludeDead,
    const bool p_bIncludePacified)
{
    if (p_nMaxCount == 0)
    {
        return {};
    }

    TEntityRef<ZActor> s_rClosestActor = {};
    float32 s_fClosestDistance = FLT_MAX;
    std::vector<std::pair<TEntityRef<ZActor>, float32>> s_aNearbyActors;
    for (auto& s_rActor : Utils::GetActors(p_bIncludeDead, p_bIncludePacified))
    {
        const auto *s_pActorSpatial = s_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_ActorWM = s_pActorSpatial->GetObjectToWorldMatrix();
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
        [](const auto &a, const auto &b)
        {
            return a.second < b.second;
        });

    if (s_aNearbyActors.size() > p_nMaxCount)
    {
        s_aNearbyActors.resize(p_nMaxCount);
    }

    return s_aNearbyActors;
}

TEntityRef<ZSpatialEntity> Utils::GetActorHeadAttachEntity(const TEntityRef<ZActor> p_rActor)
{
    if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rActor.m_entityRef))
    {
        if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x5f46597848b36b38 /* "HEAD" */); s_nIdx != -1)
        {
            if (auto* s_pHead = s_pBlueprint->GetSubEntity(p_rActor.m_entityRef.m_pObj, s_nIdx); s_pHead != nullptr)
            {
                return TEntityRef<ZSpatialEntity>(ZEntityRef(s_pHead));
            }
        }
    }

	return {};
}
