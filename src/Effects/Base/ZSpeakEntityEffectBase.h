#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZActor.h>

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Helpers/EntityWrapper.h"

/**
 * Reusable base for playing voicelines on actors.
 * Handles resource loading, unloading and spawning.
 * Use Speak() to make a actor speak, or Spawn() to 
 * spawn SpeakEntities to manage manually.
 */
class ZSpeakEntityEffectBase : public virtual IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;
	bool Available() const override;

	void OnDrawDebugUI() override;

protected:
#pragma region SpeakEntity Wrapper
	struct SSpeakEntityWrapper
	{
		WRAPPER_CONSTRUCTOR(SSpeakEntityWrapper);

		PROPERTY(ZEntityRef, m_rActor);         // actor that will speak
		PROPERTY(TEntityRef<ZActor>, m_rSpeakingActor); // while speaking, == m_rActor. null when not speaking
		PROPERTY(EActorSoundDefs, m_eSoundDef); // voiceline to play
		PROPERTY(EGestureCategory, m_eGesture); // gesture to play while speaking
		PROPERTY(bool, m_bAllowInterrupt);      // allow interruption

		INPUT_PIN(Start); // start speaking

		inline bool IsSpeaking() const
		{
			const auto s_rSpeakingActor = Getm_rSpeakingActor();
			return s_rSpeakingActor && *s_rSpeakingActor;
		}
	};
#pragma endregion

	/**
	 * Spawn a SpeakEntity bound to the given actor.
	 * @param p_rActor Actor to bind to.
	 * @param p_eSoundDef Voiceline to play.
	 * @param p_eGesture Gesture animation to use.
	 * @param p_bAllowInterrupt Allow interrupting current voiceline?
	 * @param p_bStartNow Start SpeakEntity automatically.
	*                     If false, you must Start the entity manually.
	 */
	SSpeakEntityWrapper Speak(
		const ZEntityRef& p_rActor, 
		const EActorSoundDefs p_eSoundDef, 
		const EGestureCategory p_eGesture = EGestureCategory::EGC_None, 
		const bool p_bAllowInterrupt = false,
		const bool p_bStartNow = true);

	/**
	 * Spawn a SpeakEntity. No further assignments are made.
	 */
	SSpeakEntityWrapper Spawn();

private:
	std::unique_ptr<ZTemplateEntitySpawner> m_pSpeakEntitySpawner;
	EActorSoundDefs m_eDebugSoundDef = EActorSoundDefs::Dth_BeeSting;
	EGestureCategory m_eDebugGestureCategory = EGestureCategory::EGC_None;
	bool m_bDebugAllowInterrupt = true;

	void DebugSpeakAtNearestActor();
};
