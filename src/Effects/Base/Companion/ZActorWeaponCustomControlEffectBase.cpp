#include "ZActorWeaponCustomControlEffectBase.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

SShootAtHelperBinding ZActorWeaponCustomControlEffectBase::GetShootAtHelperFor(const TEntityRef<ZActor> p_rActor) const
{
    return SShootAtHelperBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ShootAtHelper));
}
