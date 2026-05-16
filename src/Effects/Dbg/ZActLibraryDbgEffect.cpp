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
        if (const auto s_rPlayer = Utils::GetLocalPlayer())
        {
            if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                const auto s_vPlayerPos = s_pPlayerSpatial->GetObjectToWorldMatrix().Trans;

                if (const auto s_aNearby = Utils::GetNearbyActors(s_vPlayerPos, 1); !s_aNearby.empty())
                {
                    m_rTargetActor = s_aNearby.front().first;
                }
            }
        }
    }

    ImGui::TextUnformatted(fmt::format("Selected Actor: {}", m_rTargetActor ? m_rTargetActor.m_pInterfaceRef->GetActorName() : "<none>").c_str());
    if (!m_rTargetActor)
    {
        return;
    }

    DrawUIForStandWaiting();
}

void ZActLibraryDbgEffect::DrawUIForStandWaiting()
{
    ImGui::SeparatorText("ACT Stand Waiting");

    auto s_Binding = GetStandWaitingBinding(m_rTargetActor);
    auto s_bActive = s_Binding.m_bActive.value_or(false);
    auto s_bEndOnReached = s_Binding.m_bEndOnReached.value_or(false);
    auto s_fEndOnReachedDistance = s_Binding.m_fEndOnReachedDistance.value_or(0.0f);

    ImGui::Checkbox("Active", &s_bActive);
    ImGui::Checkbox("End On Reached", &s_bEndOnReached);

    if (ImGuiEx::DragFloat("End on Reached Distance", &s_fEndOnReachedDistance, 0.1f, 10.0f))
    {
        s_Binding.m_fEndOnReachedDistance = s_fEndOnReachedDistance;
    }
    
    if (ImGui::Button("Start Act"))
    {
        s_Binding.Start();
    }

    if (ImGui::Button("Cancel Act"))
    {
        s_Binding.Cancel();
    }

    if (ImGui::Button("Enable End-On-Reached"))
    {
        s_Binding.EnableEndOnReached();
    }

    if (ImGui::Button("Disable End-On-Reached"))
    {
        s_Binding.DisableEndOnReached();
    }

    if (ImGui::Button("Set Spatial to Player Position"))
    {
        if (const auto s_rPlayer = Utils::GetLocalPlayer())
        {
            if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                if (const auto s_rWaypointSpatial = s_Binding.QuerySpatial())
                {
                    s_rWaypointSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_pPlayerSpatial->GetObjectToWorldMatrix());
                }
            }
        }
    }
}

REGISTER_CHAOS_EFFECT(ZActLibraryDbgEffect);

#endif // _DEBUG
