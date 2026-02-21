#include "ZActorWeaponCustomControlEffectBase.h"

SShootAtHelperBinding ZActorWeaponCustomControlEffectBase::GetShootAtHelperFor(const TEntityRef<ZActor> p_rActor) const
{
	if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rActor.m_entityRef))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0xcafe18d8b8ef4c24 /* _ChaosMod_ShootAtHelper */); s_nIdx != -1)
		{
			if (auto* s_pHelper = s_pBlueprint->GetSubEntity(p_rActor.m_entityRef.m_pObj, s_nIdx); s_pHelper != nullptr)
			{
				return SShootAtHelperBinding(ZEntityRef(s_pHelper));
			}
		}
	}

	return {};
}
