#pragma once
#include "Helpers/EntityBinding.h"

// [modules:/zspeakentity_sounddef.class].pc_entitytype
struct SSpeakEntitySounddefBinding
{
	BINDING_CONSTRUCTOR(SSpeakEntitySounddefBinding);

	PROPERTY(ZEntityRef, m_rActor); // actor who will speak
	PROPERTY_RO(ZEntityRef, m_rSpeakingActor); // while speaking == m_rActor, null when not speaking
	PROPERTY(EGestureCategory, m_eGesture); // gesture to play while speaking
	PROPERTY(EActorSoundDefs, m_eSoundDef); // sounddef to speak
	PROPERTY(bool, m_bAllowInterrupt); // allow interrupting currently playing speech with this one
	PROPERTY(bool, m_bEnableCulling);

	INPUT_PIN(Start); // start speaking
	INPUT_PIN(Stop); // stop speaking
	//INPUT_PIN(ChangeActor);
	//INPUT_PIN(SetIActor);
	//INPUT_PIN(SetIActor_Actor);

	OUTPUT_PIN(Completed);
	OUTPUT_PIN(Done);
	OUTPUT_PIN(Aborted);

	inline bool IsSpeaking() const
	{
		const auto s_rSpeakingActor = m_rSpeakingActor;
		return s_rSpeakingActor && *s_rSpeakingActor;
	}
};
