#include "ZActorFollowPlayerHelperEffectBase.h"

SFollowPlayerHelperBinding ZActorFollowPlayerHelperEffectBase::GetFollowHelperFor(const TEntityRef<ZActor> p_rActor) const
{
	if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rActor.m_entityRef))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xcafee66c8c3ff219 /* _ChaosMod_FollowPlayerHelper */); s_nIdx != -1)
		{
			if (auto* s_pHelper = s_pBlueprint->GetSubEntity(p_rActor.m_entityRef.m_pObj, s_nIdx); s_pHelper != nullptr)
			{
				return SFollowPlayerHelperBinding(ZEntityRef(s_pHelper));
			}
		}
	}

	return {};
}
