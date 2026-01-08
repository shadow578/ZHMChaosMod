#pragma once
#include "Effects/Base/ZSpeakEntityEffectBase.h"

#include <Glacier/ZActor.h>

#include <vector>

class ZNearbyActorSpeakEffect : public ZSpeakEntityEffectBase
{
public:
	ZNearbyActorSpeakEffect(
		const std::string p_sNameSuffix, 
		const std::string p_sDisplayName,
		const std::vector<EActorSoundDefs> p_aSoundDefs,
		const float32 p_fRepetitionDelay = -1.0f,        // disable repetition when negative
		const float32 p_fRepetitionDelayVariance = 0.0f, // +- variance on repetition delay
		const float32 p_fRadius = 50.0f
	) :
		m_sNameSuffix(p_sNameSuffix),
		m_sDisplayName(p_sDisplayName),
		m_aSoundDefs(p_aSoundDefs),
		m_fRadius(p_fRadius),
		m_fRepetitionDelay(p_fRepetitionDelay),
		m_fRepetitionDelayVariance(p_fRepetitionDelayVariance),
		m_bRepeat(p_fRepetitionDelay >= 0.0f)
	{}

	void OnClearScene() override;
	void Start() override;
	void Stop() override;
	void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

	void OnDrawDebugUI() override;

	bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

	std::string GetName() const override
	{
		return ZSpeakEntityEffectBase::GetName() + "_" + m_sNameSuffix;
	}

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return m_sDisplayName;
	}

	EDuration GetDuration() const override
	{
		return m_bRepeat ? EDuration::Full : EDuration::OneShot;
	}

private:
	const std::string m_sNameSuffix;
	const std::string m_sDisplayName;
	const std::vector<EActorSoundDefs> m_aSoundDefs;
	bool m_bRepeat;
	float32 m_fRepetitionDelay;
	float32 m_fRepetitionDelayVariance;
	float32 m_fRadius;

	bool m_bActive = false;

	struct SActiveSpeakerEntry
	{
		ZEntityRef m_rActor;
		SSpeakEntityBinding m_SpeakBinding;
		float32 m_fTimeToNextRepetition;
		int32 m_nRepetitions = 0;
	};

	std::vector<SActiveSpeakerEntry> m_aActiveSpeakers;

	void FindAndRemoveSpeakersNearby();
	void RefreshActiveSpeakers(const float32 p_fDeltaTime);

	EActorSoundDefs GetRandomSoundDef() const;
	float32 GetRepetitionDelay() const;
};
