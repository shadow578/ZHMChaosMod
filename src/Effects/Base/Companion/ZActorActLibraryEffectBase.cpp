#include "ZActorActLibraryEffectBase.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

SStandWaitingActEntityBinding ZActorActLibraryEffectBase::GetStandWaitingBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SStandWaitingActEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActLibrary::StandWaiting));
}
