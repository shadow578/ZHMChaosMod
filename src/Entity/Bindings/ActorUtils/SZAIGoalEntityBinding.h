#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zaigoalentity.class].pc_entitytype
// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActorUtils/AIGoal of NPCActor, introduced by patch
// NPCActor/ActorUtils.entity.patch.json in version 1.6.0 of Companion Mod.
struct SZAIGoalEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(TArray<ZEntityRef>, m_aActors); // targeted actors. Set to parent actor in ActorUtils.
    PROPERTY(ZEntityRef, m_rTarget);         // target entity for the goal. Set to HeroStandin in ActorUtils.
    PROPERTY(EAIGoal, m_goal);               // goal the actor should pursue. NOTE: some goals may crash the game...

    INPUT_PIN(Renew);  // Start or Renew the goal for the actor(s). If the goal is not active, it will be started. if it is already active, it will remain active.
    INPUT_PIN(Expire); // Expire the goal for the actor(s).
};
