#include "ZActorActLibraryEffectBase.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

SStandWaitingActEntityBinding ZActorActLibraryEffectBase::GetStandWaitingBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SStandWaitingActEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActLibrary::StandWaiting));
}

SStandDanceMatActEntityBinding ZActorActLibraryEffectBase::GetStandDanceMatBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SStandDanceMatActEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActLibrary::StandDanceMat));
}

SLambicDanceActEntityBinding ZActorActLibraryEffectBase::GetLambicDanceBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SLambicDanceActEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActLibrary::LambicDance));
}

SFlamingoDanceActEntityBinding ZActorActLibraryEffectBase::GetFlamingoDanceBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SFlamingoDanceActEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActLibrary::FlamingoDance));
}
