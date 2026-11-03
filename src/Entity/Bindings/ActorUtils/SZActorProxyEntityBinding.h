#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zactorproxyentity.class].pc_entitytype
// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActorUtils/ActorProxy of NPCActor, introduced by patch
// NPCActor/ActorUtils.entity.patch.json in version 1.6.0 of Companion Mod.
struct SZActorProxyEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZEntityRef, m_rActor); // target actor, set to parent actor in ActorUtils patch.
    PROPERTY(ZEntityRef, m_AccidentContext);
    PROPERTY(ZEntityRef, m_rItem);
    PROPERTY(ZEntityRef, m_rSetpiece);
    PROPERTY(TArray<ZEntityRef>, m_aEnforcedOutfits);
    PROPERTY(EDamageEvent, m_eDamageEvent);
    PROPERTY(EDeathBehavior, m_eDeathBehavior);
    PROPERTY(bool, m_bMarkBodyBagAsKnown);

    INPUT_PIN(Enable);           // enable the ZActor.
    INPUT_PIN(Disable);          // disable the ZActor.
    INPUT_PIN(KillActor);        // kill the actor immediately.
    INPUT_PIN(PacifyActor);      // pacify the actor immediately.
    INPUT_PIN(KillUnnoticed);    // kill the actor immediately, in an unnoticed kill.
    INPUT_PIN(KillByAccident);   // kill the actor immediately, in an accident. hitman will not be blamed.
    INPUT_PIN(KillNotHitman);    // kill the actor immediately, in a non-hitman kill. hitman will not be blamed.
    INPUT_PIN(PacifyByAccident); // pacify the actor immediately, in an accident. hitman will not be blamed.
    INPUT_PIN(ReviveActor);      // un-pacify the actor immediately. the actor will enter a "revive" animation for ~10 seconds, then return to normal behavior.
    INPUT_PIN(DyingStart);
    INPUT_PIN(AccidentStart);
    INPUT_PIN(AccidentEnd);
    INPUT_PIN(ContextKillStart);
    INPUT_PIN(ContextKillEnd);
    INPUT_PIN(AccidentEndPacify);
    // INPUT_PIN(SetActor);  // set actor
    // INPUT_PIN(SetIActor); // set actor
    // INPUT_PIN(GetIActor);
    // INPUT_PIN(GetEntityRef);
    // INPUT_PIN(SetItem);
    // INPUT_PIN(SetSetpiece);
    INPUT_PIN(StopSpeak);
    INPUT_PIN(RuinDisguise);
    INPUT_PIN(MakeBodyKnown);
    INPUT_PIN(BagAndTag);
    // OUTPUT_PIN(OnGetIActor);
    // OUTPUT_PIN(OnGetEntityRef);
};
