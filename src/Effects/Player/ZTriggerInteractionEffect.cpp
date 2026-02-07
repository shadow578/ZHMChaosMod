#include "ZTriggerInteractionEffect.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZTriggerInteractionEffect] "

void ZTriggerInteractionEffect::OnEnterScene()
{
	const auto &s_aInteractions = Utils::ZEntityFinder()
																		.BlueprintResource<"[assembly:/_pro/design/gamecore/interactionbase.template?/interaction_subaction_noinvestigate.entitytemplate].pc_entityblueprint">()
																		.Find();

	for (auto &s_rEntity : s_aInteractions)
	{
		SInteractionSubactionEntityBinding s_Binding(s_rEntity);
		auto s_rContextObjectSpatial = s_Binding.m_rContextObjectSpatial;
		if (!s_rContextObjectSpatial || !s_rContextObjectSpatial.value())
		{
			continue;
		}

		m_aInteractionEntities.push_back(s_Binding);
	}

	Logger::Debug(TAG "Found {} interaction entities.", m_aInteractionEntities.size());
}

void ZTriggerInteractionEffect::OnClearScene()
{
	m_aInteractionEntities.clear();
}

bool ZTriggerInteractionEffect::Available() const
{
	return IChaosEffect::Available() && !m_aInteractionEntities.empty();
}

void ZTriggerInteractionEffect::Start()
{
	const auto s_rPlayer = SDK()->GetLocalPlayer();
	if (!s_rPlayer)
	{
		return;
	}

	const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>();
	if (!s_pPlayerSpatial)
	{
		return;
	}

	const auto s_vPlayerPosition = s_pPlayerSpatial->GetObjectToWorldMatrix().Trans;

	std::vector<SInteractionSubactionEntityBinding> s_aNearbyInteractions;
	for (auto &s_rBinding : m_aInteractionEntities)
	{
		const auto s_rContextObjectSpatial = s_rBinding.m_rContextObjectSpatial;
		if (!s_rContextObjectSpatial || !s_rContextObjectSpatial.value())
		{
			continue;
		}

		const auto s_vInteractionPosition = s_rContextObjectSpatial.value().m_pInterfaceRef->GetObjectToWorldMatrix().Trans;

		const auto s_fDistance = float4::Distance(s_vPlayerPosition, s_vInteractionPosition);

		if (s_fDistance <= m_fRadius)
		{
			s_aNearbyInteractions.push_back(s_rBinding);
		}
	}

	SInteractionSubactionEntityBinding s_Interaction;
	if (!s_aNearbyInteractions.empty())
	{
		for (int i = 0; i < 10; i++)
		{
			s_Interaction = Math::SelectRandomElement(s_aNearbyInteractions);

			// "Leave Mission" interactions tend to break the game
			if (s_Interaction.m_sPromptText.value_or("") != "Leave Mission")
			{
				break;
			}
		}
	}
	else
	{
		Logger::Debug(TAG "No interaction entities found within radius {}, selecting from all available!", m_fRadius);
		s_Interaction = Math::SelectRandomElement(m_aInteractionEntities);
	}

	// trigger!
	s_Interaction.Execute();

	const std::string s_sPromptText = s_Interaction.m_sPromptText.value_or("").c_str();
	const std::string s_sPromptDescriptionText = s_Interaction.m_sPromptDescriptionText.value_or("").c_str();

	if (s_sPromptText.empty() && s_sPromptDescriptionText.empty())
	{
		m_sLastInteractionText = "";
	}
	else
	{
		m_sLastInteractionText = s_sPromptText;
		if (!s_sPromptDescriptionText.empty())
		{
			m_sLastInteractionText += " - " + s_sPromptDescriptionText;
		}
	}

	Logger::Debug(TAG "Triggered interaction: {}", m_sLastInteractionText);
}

REGISTER_CHAOS_EFFECT_PARAM(world, ZTriggerInteractionEffect, "world", "Trigger Random Interaction", 9999.0f);
// REGISTER_CHAOS_EFFECT_PARAM(nearby, ZTriggerInteractionEffect, "nearby", "Trigger Nearby Interaction", 50.0f);
