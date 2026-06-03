#include "ZTriggerInteractionEffect.h"

#include <Logging.h>
#include <imgui.h>

#include <Glacier/SGameUpdateEvent.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/Math.h"
#include "ZConfigurationAccessor.h"

#define TAG "[ZTriggerInteractionEffect] "

void ZTriggerInteractionEffect::OnEnterScene()
{
    const auto& s_aInteractions = Utils::ZEntityFinder()
                                      .BlueprintResource<"[assembly:/_pro/design/gamecore/interactionbase.template?/interaction_subaction_noinvestigate.entitytemplate].pc_entityblueprint">()
                                      .Find();

    for (auto& s_rEntity : s_aInteractions)
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
    SMatrix s_mPlayerTransform;
    if (!Utils::GetPlayerTransform(s_mPlayerTransform))
    {
        return;
    }
    const auto s_vPlayerPosition = s_mPlayerTransform.Pos;

    std::vector<SInteractionSubactionEntityBinding> s_aNearbyInteractions;
    for (auto& s_rBinding : m_aInteractionEntities)
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
    auto s_bIsInteractionValid = false;
    if (!s_aNearbyInteractions.empty())
    {
        for (int i = 0; i < 50; i++)
        {
            s_Interaction = Math::SelectRandomElement(s_aNearbyInteractions);

            if (m_bOnlyUseableInteractions)
            {
                if (const auto s_Condition = s_Interaction.GetVisibleAndUseableCondition())
                {
                    if (!s_Condition.m_bValue.value_or(false))
                    {
                        continue;
                    }
                }
            }

            if (const auto s_Condition = s_Interaction.GetVisibleCondition())
            {
                if (!s_Condition.m_bValue.value_or(false))
                {
                    continue;
                }
            }

            // "Leave Mission" interactions tend to break the game
            if (s_Interaction.m_sPromptText.value_or("") == "Leave Mission")
            {
                continue;
            }

            // "Disguise" interactions can crash
            if (s_Interaction.m_sPromptText.value_or("") == "Disguise")
            {
                continue;
            }

            s_bIsInteractionValid = true;
            break;
        }
    }
    else
    {
        Logger::Debug(TAG "No interaction entities found within radius {}, selecting from all available!", m_fRadius);
        s_Interaction = Math::SelectRandomElement(m_aInteractionEntities);
    }

    if (!s_bIsInteractionValid)
    {
        Logger::Debug(TAG "Couldn't find a valid interaction, aborting!");
        return;
    }

    m_CurrentInteraction = s_Interaction;
    m_fTimeToInteractionStart = .5f; // need some delay to allow for teleport to happen

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

    if (auto s_roContextObjectSpatial = s_Interaction.m_rContextObjectSpatial; s_roContextObjectSpatial.has_value() && s_roContextObjectSpatial.value())
    {
        const auto s_mTrans = s_roContextObjectSpatial->m_pInterfaceRef->GetObjectToWorldMatrix();
        Utils::TeleportPlayer(s_mTrans.Pos); // preserve player rotation
    }

    Logger::Debug(TAG "Queued interaction trigger: {}", m_sLastInteractionText);
}

void ZTriggerInteractionEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (m_fTimeToInteractionStart > 0.f)
    {
        m_fTimeToInteractionStart -= static_cast<float32>(p_UpdateEvent.m_GameTimeDelta.ToSeconds());
        if (m_fTimeToInteractionStart <= 0.f)
        {
            m_fTimeToInteractionStart = -1.f;
            m_CurrentInteraction.Execute();
            Logger::Debug(TAG "Trigger Interaction: {}", m_sLastInteractionText);
        }
    }
}

void ZTriggerInteractionEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    m_bOnlyUseableInteractions = p_pConfiguration->GetBool("OnlyUseable", m_bOnlyUseableInteractions);
}

void ZTriggerInteractionEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    if (ImGui::Checkbox("Only Useable Interactions", &m_bOnlyUseableInteractions))
    {
        p_pConfiguration->SetBool("OnlyUseable", m_bOnlyUseableInteractions);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("If enabled, only interactions that are currently useable (e.g. required item is present) will be triggered. Otherwise, any interaction can be triggered, which may cause issues during the animation or (rare) game crashes.");
    }
}

REGISTER_CHAOS_EFFECT_PARAM(world, ZTriggerInteractionEffect, "world", "Trigger Random Interaction", 9999.0f);
REGISTER_CHAOS_EFFECT_PARAM(nearby, ZTriggerInteractionEffect, "nearby", "Trigger Nearby Interaction", 50.0f);
