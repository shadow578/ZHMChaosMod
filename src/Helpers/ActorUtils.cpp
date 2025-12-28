#include "ActorUtils.h"
#include "Math.h"

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
