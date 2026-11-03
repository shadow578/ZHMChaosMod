#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Entity/Bindings/ActorUtils/SZActorAIPerceptibilityDisableEntityBinding.h"
#include "Entity/Bindings/ActorUtils/SZActorProxyEntityBinding.h"
#include "Entity/Bindings/ActorUtils/SZAIGoalEntityBinding.h"
#include "Entity/Bindings/ActorUtils/SZAIModifierBinding.h"
#include "Entity/Bindings/ActorUtils/SZRagdollAttacherEntityBinding.h"
#include "Entity/Bindings/ActorUtils/SZSpeakEntitySounddefBinding.h"

/**
 * Base class for effects that use the NPCActor misc. ActorUtils from Companion Mod.
 */
class ZActorUtilsEffectBase : public virtual ZCompanionModDependentEffectBase
{
  public:
    ZActorUtilsEffectBase()
        : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 6, 0)) // NPCActor ActorUtils added
    {
    }

  protected:
    SZActorAIPerceptibilityDisableEntityBinding GetActorAIPerceptibilityDisabledBinding(const TEntityRef<ZActor> p_rActor) const;
    SZActorProxyEntityBinding GetActorProxyBinding(const TEntityRef<ZActor> p_rActor) const;
    SZAIGoalEntityBinding GetAIGoalBinding(const TEntityRef<ZActor> p_rActor) const;
    SZAIModifierActorBinding GetAIModifierBinding(const TEntityRef<ZActor> p_rActor) const;
    SZRagdollAttacherEntityBinding GetRagdollAttacherBinding(const TEntityRef<ZActor> p_rActor) const;
    SZSpeakEntitySounddefBinding GetSpeakEntityBinding(const TEntityRef<ZActor> p_rActor) const;
};
