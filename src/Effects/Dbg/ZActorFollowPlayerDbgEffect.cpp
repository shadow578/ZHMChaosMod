#ifdef _DEBUG
#include "ZActorFollowPlayerDbgEffect.h"

#include <imgui.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZActorFollowPlayerDbgEffect] "

void ZActorFollowPlayerDbgEffect::OnDrawDebugUI()
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
        ImGui::TextUnformatted("Select a target actor to get started!");
        return;
    }

    auto s_Binding = GetFollowHelperFor(m_rTargetActor);
    if (!s_Binding)
    {
        ImGui::TextUnformatted("Did not get follow helper!");
        return;
    }

    if (auto s_foMinTetherRange = s_Binding.m_fMinTetherRange; s_foMinTetherRange.has_value())
    {
        auto s_fMinTetherRange = s_foMinTetherRange.value();
        if (ImGui::DragFloat("Min Tether Range", &s_fMinTetherRange))
        {
            s_Binding.m_fMinTetherRange = s_fMinTetherRange;
        }
    }

    if (auto s_foMaxTetherRange = s_Binding.m_fMaxTetherRange; s_foMaxTetherRange.has_value())
    {
        auto s_fMaxTetherRange = s_foMaxTetherRange.value();
        if (ImGui::DragFloat("Max Tether Range", &s_fMaxTetherRange))
        {
            s_Binding.m_fMaxTetherRange = s_fMaxTetherRange;
        }
    }

    if (auto s_foMaxSightDistance = s_Binding.m_fMaxSightDistance; s_foMaxSightDistance.has_value())
    {
        auto s_fMaxSightDistance = s_foMaxSightDistance.value();
        if (ImGui::DragFloat("Max Sight Range", &s_fMaxSightDistance))
        {
            s_Binding.m_fMaxSightDistance = s_fMaxSightDistance;
        }
    }

    if (auto s_soMatch = s_Binding.m_sMatch; s_soMatch.has_value())
    {
        std::string s_sMatch = s_soMatch.value().c_str();
        ImGui::TextUnformatted(fmt::format("Behavior Tree Match Node: {}", s_sMatch).c_str());
    }

    if (ImGui::Button("StartFollow (normal)"))
    {
        s_Binding.StartFollowHitman();
    }
    ImGui::SameLine();
    if (ImGui::Button("StopFollow (normal)"))
    {
        s_Binding.StopFollowHitman();
    }

    if (ImGui::Button("StartFollow (ignore everything)"))
    {
        s_Binding.StartFollowHitmanIgnoreEverything();
    }
    ImGui::SameLine();
    if (ImGui::Button("StopFollow (ignore everything)"))
    {
        s_Binding.StopFollowHitmanIgnoreEverything();
    }
}

REGISTER_CHAOS_EFFECT(ZActorFollowPlayerDbgEffect);

#endif // _DEBUG
