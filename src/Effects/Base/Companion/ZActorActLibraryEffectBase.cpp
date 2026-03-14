#include "ZActorActLibraryEffectBase.h"

SActLibraryBinding ZActorActLibraryEffectBase::GetActLibraryFor(const TEntityRef<ZActor> p_rActor) const
{
    if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rActor.m_entityRef))
    {
        if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xcafe5bb2416f6e81 /* _ChaosMod_ActLibrary */); s_nIdx != -1)
        {
            if (auto* s_pHelper = s_pBlueprint->GetSubEntity(p_rActor.m_entityRef.m_pObj, s_nIdx); s_pHelper != nullptr)
            {
                return SActLibraryBinding(ZEntityRef(s_pHelper));
            }
        }
    }

    return {};
}

SDanceMatActBinding ZActorActLibraryEffectBase::GetDanceMatActFor(const TEntityRef<ZActor> p_rActor) const
{
    if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rActor.m_entityRef))
    {
        if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xcafeea36ac1680ff /* _ChaosMod_ActLibrary/DanceMat */); s_nIdx != -1)
        {
            if (auto* s_pHelper = s_pBlueprint->GetSubEntity(p_rActor.m_entityRef.m_pObj, s_nIdx); s_pHelper != nullptr)
            {
                return SDanceMatActBinding(ZEntityRef(s_pHelper));
            }
        }
    }

    return {};
}
