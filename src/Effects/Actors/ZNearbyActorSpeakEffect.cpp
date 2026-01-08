#include "ZNearbyActorSpeakEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/Math.h"
#include "Helpers/Utils.h"
#include "Helpers/ImGuiExtras.h"

void ZNearbyActorSpeakEffect::OnClearScene()
{
	m_aActiveSpeakers.clear();
	m_bActive = false;
	ZSpeakEntityEffectBase::OnClearScene();

}

void ZNearbyActorSpeakEffect::OnDrawDebugUI()
{
	ImGuiEx::DragFloat("Radius", &m_fRadius, 1.0f, 500.0f);
	ImGui::Checkbox("Repeat", &m_bRepeat);

	ImGui::BeginDisabled(!m_bRepeat);

	ImGuiEx::DragFloat("Repetition Delay", &m_fRepetitionDelay, 0.0f, 30.0f, "%.1f s");
	ImGuiEx::DragFloat("Repetition Delay Variance", &m_fRepetitionDelayVariance, 0.0f, 15.0f, "%.1f s");

	ImGui::EndDisabled();

	ImGui::Separator();
	ImGui::TextUnformatted(fmt::format("Active Speakers #: {}", m_aActiveSpeakers.size()).c_str());

	ImGui::SeparatorText("ZSpeakEntityEffectBase");
	ZSpeakEntityEffectBase::OnDrawDebugUI();
}

bool ZNearbyActorSpeakEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
	return ZSpeakEntityEffectBase::IsCompatibleWith(p_pOtherEffect) &&
		!Utils::IsInstanceOf<ZSpeakEntityEffectBase>(p_pOtherEffect);
}

void ZNearbyActorSpeakEffect::Start()
{
	m_aActiveSpeakers.clear();
	FindAndRemoveSpeakersNearby();

	m_bActive = true;
}

void ZNearbyActorSpeakEffect::Stop()
{
	m_aActiveSpeakers.clear();
	m_bActive = false;
}

void ZNearbyActorSpeakEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	// only find new speakers if repeating
	if (m_bRepeat)
	{
		FindAndRemoveSpeakersNearby();
	}

	// refresh always (deferred start)
	RefreshActiveSpeakers(p_fDeltaTime);
}

void ZNearbyActorSpeakEffect::FindAndRemoveSpeakersNearby()
{
	const auto s_rPlayer = SDK()->GetLocalPlayer();
	if (!s_rPlayer)
	{
		return;
	}

	const auto s_pPlayerSpatial = s_rPlayer.m_ref.QueryInterface<ZSpatialEntity>();
	if (!s_pPlayerSpatial)
	{
		return;
	}

	const auto s_vPlayerPosition = s_pPlayerSpatial->GetWorldMatrix().Trans;

	for (auto& [s_rActor, s_fDistance] : Utils::GetNearbyActors(s_vPlayerPosition))
	{
		// if far away, attempt to remove 
		if (s_fDistance > m_fRadius)
		{
			auto s_nIt = std::remove_if(
				m_aActiveSpeakers.begin(),
				m_aActiveSpeakers.end(),
				[&s_rActor](const SActiveSpeakerEntry& p_Entry)
				{
					return p_Entry.m_rActor == s_rActor;
				}
			);
			m_aActiveSpeakers.erase(s_nIt, m_aActiveSpeakers.end());
			continue;
		}

		// check if already speaking
		const bool s_bExists = std::any_of(
			m_aActiveSpeakers.begin(),
			m_aActiveSpeakers.end(),
			[&s_rActor](const SActiveSpeakerEntry& p_Entry)
			{
				return p_Entry.m_rActor == s_rActor;
			}
		);
		if (s_bExists)
		{
			continue;
		}

		// start speaking
		const auto s_SpeakerWrapper = Speak(
			s_rActor,
			GetRandomSoundDef(),
			EGestureCategory::EGC_None,
			true,
			false // defer start
		);

		const SActiveSpeakerEntry s_NewEntry{
			.m_rActor = s_rActor,
			.m_SpeakBinding = s_SpeakerWrapper,
			.m_fTimeToNextRepetition = GetRepetitionDelay()
		};
		m_aActiveSpeakers.push_back(s_NewEntry);
	}
}

void ZNearbyActorSpeakEffect::RefreshActiveSpeakers(const float32 p_fDeltaTime)
{
	for (auto& s_ActiveSpeaker : m_aActiveSpeakers)
	{
		auto& s_SpeakWrapper = s_ActiveSpeaker.m_SpeakBinding;
		if (!s_SpeakWrapper.IsSpeaking())
		{
			if (!m_bRepeat && s_ActiveSpeaker.m_nRepetitions > 0)
			{
				// one-shot and already spoke once, skip
				continue;
			}

			if (s_ActiveSpeaker.m_fTimeToNextRepetition > 0.0f)
			{
				s_ActiveSpeaker.m_fTimeToNextRepetition -= p_fDeltaTime;
				continue;
			}

			// restart with new sound pick
			s_SpeakWrapper.m_eSoundDef = GetRandomSoundDef();
			s_SpeakWrapper.Start();

			s_ActiveSpeaker.m_fTimeToNextRepetition = GetRepetitionDelay();
			s_ActiveSpeaker.m_nRepetitions++;
		}
	}
}

EActorSoundDefs ZNearbyActorSpeakEffect::GetRandomSoundDef() const
{
	if (m_aSoundDefs.empty())
	{
		return EActorSoundDefs::Dth_BeeSting;
	}
	
	return Math::SelectRandomElement(m_aSoundDefs);
}

float32 ZNearbyActorSpeakEffect::GetRepetitionDelay() const
{
	if (m_fRepetitionDelayVariance <= 0.0f)
	{
		return m_fRepetitionDelay;
	}

	// special case: when repetition is disabled, variance controls start delay
	if (m_fRepetitionDelay < 0.0f)
	{
		return Math::GetRandomNumber<float32>(0.0f, m_fRepetitionDelayVariance);
	}

	const float32 s_fVariance = Math::GetRandomNumber<float32>(-m_fRepetitionDelayVariance, m_fRepetitionDelayVariance);
	const float32 s_fDelay = m_fRepetitionDelay + s_fVariance;
	return max(s_fDelay, 0.0f);
}

REGISTER_CHAOS_EFFECT_PARAM(curse, ZNearbyActorSpeakEffect, "curse", 
	"Cussocalypse", 
	std::vector<EActorSoundDefs>{ 
		EActorSoundDefs::Gen_Curse,    // cursing
		EActorSoundDefs::Gen_CurseLow, // less intense cursing
		EActorSoundDefs::Gen_CoinCurse // curse at coin (?)
	},
	1.0f, // repetition
	0.5f  // variance
);

REGISTER_CHAOS_EFFECT_PARAM(scream, ZNearbyActorSpeakEffect, "scream",
	"Aaaaaah!",
	std::vector<EActorSoundDefs>{ 
		EActorSoundDefs::Dth_Fll,  // fall scream
		EActorSoundDefs::Dth_Xplo, // explosion scream
		EActorSoundDefs::Cmbt_Scrm // combat scream
	},
	0.5f,
	0.2f
);

REGISTER_CHAOS_EFFECT_PARAM(cough, ZNearbyActorSpeakEffect, "cough",
	"Pandemic Mode",
	std::vector<EActorSoundDefs>{ 
		EActorSoundDefs::Exp_Cough,  // *cough*
		EActorSoundDefs::Exp_Cough,
		EActorSoundDefs::Exp_Cough,
		EActorSoundDefs::Exp_ClearThroat // (rarely) *clears throat*
	},
	2.0f,
	1.0f
);

REGISTER_CHAOS_EFFECT_PARAM(mean, ZNearbyActorSpeakEffect, "mean",
	"Mean Actors", // TODO better name :D
	std::vector<EActorSoundDefs>{ 
		EActorSoundDefs::Gen_SmellAck,  // "Phew, you stink!"
		EActorSoundDefs::Gen_SmellAck,
		EActorSoundDefs::Cmbt_HMMssTnt, // combat taunt, "this guy can't hit anything!"
		EActorSoundDefs::Cmbt_HMMssTnt,
		EActorSoundDefs::InCa_ClstTnt,  // hiding taunt, "are we playing peekaboo?"
		EActorSoundDefs::InCa_ClstTnt,
		EActorSoundDefs::InSt_HM2Cls,   // too close banter
		EActorSoundDefs::InSt_HM2Cls,
		EActorSoundDefs::InSt_HM2Cls,
		EActorSoundDefs::InDsg_FllwWrn1Nkd, // "Are those my pants?!"
		EActorSoundDefs::Gen_NkdRunAck  // ;)
	},
	8.0f,
	3.0f
);

REGISTER_CHAOS_EFFECT_PARAM(bees, ZNearbyActorSpeakEffect, "bees",
	"Bees!",
	std::vector<EActorSoundDefs>{
		EActorSoundDefs::Dth_BeeSting // "i was stung by a bee!"
	},
	-1.0f, // no repetition
	2.0f
);
