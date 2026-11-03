#include "ZActorUtilsEffectBase.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

SZActorAIPerceptibilityDisableEntityBinding ZActorUtilsEffectBase::GetActorAIPerceptibilityDisabledBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZActorAIPerceptibilityDisableEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::ActorAIPerceptibilityDisable));
}

SZActorProxyEntityBinding ZActorUtilsEffectBase::GetActorProxyBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZActorProxyEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::ActorProxy));
}

SZAIGoalEntityBinding ZActorUtilsEffectBase::GetAIGoalBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZAIGoalEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::AIGoal));
}

SZAIModifierActorBinding ZActorUtilsEffectBase::GetAIModifierBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZAIModifierActorBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::AIModifierActor));
}

SZRagdollAttacherEntityBinding ZActorUtilsEffectBase::GetRagdollAttacherBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZRagdollAttacherEntityBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::RagdollAttacher));
}

SZSpeakEntitySounddefBinding ZActorUtilsEffectBase::GetSpeakEntityBinding(const TEntityRef<ZActor> p_rActor) const
{
    return SZSpeakEntitySounddefBinding(Utils::GetSubEntity(p_rActor.m_entityRef, EntityId::CompanionMod::NPCActor::ActorUtils::SpeakEntitySoundDef));
}
