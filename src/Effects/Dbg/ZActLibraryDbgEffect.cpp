#ifdef _DEBUG
#include "ZActLibraryDbgEffect.h"

#include <imgui.h>

#include <Glacier/ZHitman5.h>
#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/ImGuiExtras.h"

#define TAG "[ZActLibraryDbgEffect] "

void ZActLibraryDbgEffect::OnDrawDebugUI()
{
    if (ImGui::Button("Select Nearest Actor"))
    {
        SMatrix s_mPlayerTransform;
        if (Utils::GetPlayerTransform(s_mPlayerTransform))
        {
            const auto s_vPlayerPosition = s_mPlayerTransform.Pos;
            if (const auto s_aNearby = Utils::GetNearbyActors(s_vPlayerPosition, 1); !s_aNearby.empty())
            {
                m_rTargetActor = s_aNearby.front().first;
            }
        }
    }

    ImGui::TextUnformatted(fmt::format("Selected Actor: {}", m_rTargetActor ? m_rTargetActor.m_pInterfaceRef->GetActorName() : "<none>").c_str());
    if (!m_rTargetActor)
    {
        return;
    }

    DrawUIForStandWaiting();
    DrawUIForStandDanceMat();
    DrawUIForLambicDance();
    DrawUIForFlamingoDance();
}

static inline std::string_view SituationTypeToName(const ESituationAvailability p_eSituation)
{
    switch (p_eSituation)
    {
    case ESituationAvailability::ESA_AMBIENCE:
        return "ESA_AMBIENCE";
    case ESituationAvailability::ESA_AMBIENCE_RESV:
        return "ESA_AMBIENCE_RESV";
    case ESituationAvailability::ESA_OVR_STANDING:
        return "ESA_OVR_STANDING";
    case ESituationAvailability::ESA_OVR_CURIOUS:
        return "ESA_OVR_CURIOUS";
    case ESituationAvailability::ESA_OVR_SENTRY:
        return "ESA_OVR_SENTRY";
    case ESituationAvailability::ESA_OVR_CAUTIOUS:
        return "ESA_OVR_CAUTIOUS";
    case ESituationAvailability::ESA_OVR_COMBAT:
        return "ESA_OVR_COMBAT";
    case ESituationAvailability::ESA_OVR_ALL:
        return "ESA_OVR_ALL";
    default:
        return "<unknown>";
    }
}

/**
 * Draw UI for common act library bindings:
 * - Active
 * - Start Act
 * - Cancel Act
 * - Set Spatial to Player Position
 * - Situation type
 * - Movement type
 */
template <typename T>
static inline void DrawCommonBindingUI(T& p_Binding)
{
    if constexpr (requires { p_Binding.m_bActive; })
    {
        auto s_bActive = p_Binding.m_bActive.value_or(false);
        ImGui::Checkbox("Active", &s_bActive);
    }

    if constexpr (requires { p_Binding.m_MovementType; })
    {
        const auto s_eMovementType = p_Binding.m_MovementType.value_or(ZActBehaviorEntity_EMovementType::MT_WALK);
        auto s_bMovementTypeSnap = (s_eMovementType == ZActBehaviorEntity_EMovementType::MT_SNAP);
        if (ImGui::Checkbox("Movement Type MT_SNAP?", &s_bMovementTypeSnap))
        {
            p_Binding.m_MovementType = s_bMovementTypeSnap ? ZActBehaviorEntity_EMovementType::MT_SNAP : ZActBehaviorEntity_EMovementType::MT_WALK;
        }
    }

    if constexpr (requires { p_Binding.m_eSituationType; })
    {
        static const std::vector<ESituationAvailability> s_aSituationTypes = {
            ESituationAvailability::ESA_AMBIENCE,
            ESituationAvailability::ESA_AMBIENCE_RESV,
            ESituationAvailability::ESA_OVR_STANDING,
            ESituationAvailability::ESA_OVR_CURIOUS,
            ESituationAvailability::ESA_OVR_SENTRY,
            ESituationAvailability::ESA_OVR_CAUTIOUS,
            ESituationAvailability::ESA_OVR_COMBAT,
            ESituationAvailability::ESA_OVR_ALL,
        };

        const auto s_eSituationType = p_Binding.m_eSituationType.value_or(ESituationAvailability::ESA_AMBIENCE);
        if (ImGui::BeginCombo("Situation Type", SituationTypeToName(s_eSituationType).data()))
        {
            for (const auto s_eType : s_aSituationTypes)
            {
                const bool s_bSelected = (s_eType == s_eSituationType);
                if (ImGui::Selectable(SituationTypeToName(s_eType).data(), s_bSelected))
                {
                    p_Binding.m_eSituationType = s_eType;
                }
            }

            ImGui::EndCombo();
        }
    }

    if constexpr (requires { p_Binding.Start(); })
    {
        if (ImGui::Button("Start Act"))
        {
            p_Binding.Start();
        }
    }

    if constexpr (requires { p_Binding.Cancel(); })
    {
        if (ImGui::Button("Cancel Act"))
        {
            p_Binding.Cancel();
        }
    }

    if constexpr (requires { p_Binding.QuerySpatial(); })
    {
        if (ImGui::Button("Set Spatial to Player Position"))
        {
            SMatrix s_mPlayerTransform;
            if (Utils::GetPlayerTransform(s_mPlayerTransform))
            {
                if (const auto s_rWaypointSpatial = p_Binding.QuerySpatial())
                {
                    s_rWaypointSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mPlayerTransform);
                }
            }
        }
    }
}

void ZActLibraryDbgEffect::DrawUIForStandWaiting()
{
    ImGui::PushID("##stand_waiting");

    if (ImGui::CollapsingHeader("Act_MR_Stand_Waiting"))
    {
        auto s_Binding = GetStandWaitingBinding(m_rTargetActor);
        auto s_bEndOnReached = s_Binding.m_bEndOnReached.value_or(false);
        auto s_fEndOnReachedDistance = s_Binding.m_fEndOnReachedDistance.value_or(0.0f);

        ImGui::Checkbox("End On Reached", &s_bEndOnReached);

        if (ImGuiEx::DragFloat("End on Reached Distance", &s_fEndOnReachedDistance, 0.1f, 10.0f))
        {
            s_Binding.m_fEndOnReachedDistance = s_fEndOnReachedDistance;
        }

        if (ImGui::Button("Enable End-On-Reached"))
        {
            s_Binding.EnableEndOnReached();
        }

        if (ImGui::Button("Disable End-On-Reached"))
        {
            s_Binding.DisableEndOnReached();
        }

        DrawCommonBindingUI(s_Binding);
    }

    ImGui::PopID();
}

void ZActLibraryDbgEffect::DrawUIForStandDanceMat()
{
    ImGui::PushID("##stand_dance_mat");

    if (ImGui::CollapsingHeader("Act_MR_Stand_Dance_Mat"))
    {
        auto s_Binding = GetStandDanceMatBinding(m_rTargetActor);
        auto s_bExpertMode = s_Binding.m_bExpertMode.value_or(false);

        ImGui::Checkbox("Expert Mode", &s_bExpertMode);

        if (ImGui::Button("Enable Expert Mode"))
        {
            s_Binding.SetExpertMode();
        }

        if (ImGui::Button("Disable Expert Mode"))
        {
            s_Binding.SetNormalMode();
        }

        DrawCommonBindingUI(s_Binding);
    }

    ImGui::PopID();
}

void ZActLibraryDbgEffect::DrawUIForLambicDance()
{
    ImGui::PushID("##lambic_dance");

    if (ImGui::CollapsingHeader("Act_MR_Lambic_Dance"))
    {
        auto s_Binding = GetLambicDanceBinding(m_rTargetActor);

        DrawCommonBindingUI(s_Binding);
    }

    ImGui::PopID();
}

void ZActLibraryDbgEffect::DrawUIForFlamingoDance()
{
    ImGui::PushID("##flamingo_dance");

    if (ImGui::CollapsingHeader("Act_MR_Stand_Mascot_Entertain"))
    {
        auto s_Binding = GetFlamingoDanceBinding(m_rTargetActor);
        auto s_nMode = s_Binding.m_nMode.value_or(0);

        if (ImGui::InputInt("Mode", &s_nMode))
        {
            s_Binding.m_nMode = s_nMode;
        }

        DrawCommonBindingUI(s_Binding);
    }

    ImGui::PopID();
}

REGISTER_CHAOS_EFFECT(ZActLibraryDbgEffect);

#endif // _DEBUG
