#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZActor.h>

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Helpers/EntityBindings/SSpeakEntitySounddefBinding.h"

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
	/**
	 * Spawn a SpeakEntity bound to the given actor.
	 * @param p_rActor Actor to bind to.
	 * @param p_eSoundDef Voiceline to play.
	 * @param p_eGesture Gesture animation to use.
	 * @param p_bAllowInterrupt Allow interrupting current voiceline?
	 * @param p_bStartNow Start SpeakEntity automatically.
	 *                     If false, you must Start the entity manually.
	 */
	SSpeakEntitySounddefBinding Speak(
			const ZEntityRef &p_rActor,
			const EActorSoundDefs p_eSoundDef,
			const EGestureCategory p_eGesture = EGestureCategory::EGC_None,
			const bool p_bAllowInterrupt = false,
			const bool p_bStartNow = true);

	/**
	 * Spawn a SpeakEntity. No further assignments are made.
	 */
	SSpeakEntitySounddefBinding Spawn();

private:
	std::unique_ptr<ZTemplateEntitySpawner> m_pSpeakEntitySpawner;
	EActorSoundDefs m_eDebugSoundDef = EActorSoundDefs::Dth_BeeSting;
	EGestureCategory m_eDebugGestureCategory = EGestureCategory::EGC_None;
	bool m_bDebugAllowInterrupt = true;

	void DebugSpeakAtNearestActor();
};
