#pragma once
#include "Entity/Bindings/EntityBinding.h"

class ZActor;

// [modules:/zspeakentity_sounddef.class].pc_entitytype
// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActorUtils/SpeakEntity_SoundDef of NPCActor, introduced by patch
// NPCActor/ActorUtils.entity.patch.json in version 1.6.0 of Companion Mod.
struct SZSpeakEntitySounddefBinding : SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZEntityRef, m_rActor);                    // actor who will speak. Set to parent actor in ActorUtils.
    PROPERTY_RO(TEntityRef<ZActor>, m_rSpeakingActor); // while speaking == m_rActor, null when not speaking
    PROPERTY(EGestureCategory, m_eGesture);            // gesture to play while speaking
    PROPERTY(EActorSoundDefs, m_eSoundDef);            // sounddef to speak
    PROPERTY(bool, m_bAllowInterrupt);                 // allow interrupting currently playing speech with this one
    PROPERTY(bool, m_bEnableCulling);

    INPUT_PIN(Start); // start speaking
    INPUT_PIN(Stop);  // stop speaking
    // INPUT_PIN(ChangeActor);
    // INPUT_PIN(SetIActor);
    // INPUT_PIN(SetIActor_Actor);

    // OUTPUT_PIN(Completed);
    // OUTPUT_PIN(Done);
    // OUTPUT_PIN(Aborted);

    inline bool IsSpeaking() const
    {
        const auto s_rSpeakingActor = m_rSpeakingActor;
        return s_rSpeakingActor && *s_rSpeakingActor;
    }
};
