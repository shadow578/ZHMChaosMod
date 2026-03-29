#include "ZActorFollowPlayerHelperEffectBase.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

SFollowPlayerHelperBinding ZActorFollowPlayerHelperEffectBase::GetFollowHelperFor(const TEntityRef<ZActor> p_rActor) const
{
    return SFollowPlayerHelperBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::FollowPlayerHelper));
}
