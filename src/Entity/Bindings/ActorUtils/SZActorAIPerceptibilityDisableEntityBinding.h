#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zactoraiperceptibilitydisable.class].pc_entitytype
// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActorUtils/ActorAIPerceptibilityDisable of NPCActor, introduced by patch
// NPCActor/ActorUtils.entity.patch.json in version 1.6.0 of Companion Mod.
struct SZActorAIPerceptibilityDisableEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    // from ZActorAIPerceptibilityDisable entity
    PROPERTY(ZEntityRef, m_rActor);              // target actor, set to parent actor in ActorUtils patch.
    PROPERTY_RO(ZEntityRef, m_rEnableCondition); // enable condition ValueBool reference.

    // propertyAlias added in ActorUtils
    PROPERTY(bool, m_bDisablePerceptibility); // m_bEnable of m_rEnableCondition ValueBool. Set to true to disable perceptibility, false to (re-)enable it.
};
