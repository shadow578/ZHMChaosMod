#include "ZHUDImageVideoViewEffectBase.h"

#include <imgui.h>

void ZHUDImageVideoViewEffectBase::LoadResources()
{
    m_pViewSpawner = ZTemplateEntitySpawner::Create<"[assembly:/_pro/chaosmod/hud_image_video_view.entitytemplate].pc_entitytype">();
}

void ZHUDImageVideoViewEffectBase::OnClearScene()
{
    m_rLastSpawnedViewEntity = {};
    m_pViewSpawner = nullptr;
}

bool ZHUDImageVideoViewEffectBase::Available() const
{
    return ZCompanionModDependentEffectBase::Available()
           && m_pViewSpawner && m_pViewSpawner->IsAvailable();
}

void ZHUDImageVideoViewEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pViewSpawner->ToString()).c_str());

    auto s_View = SImageVideoViewEntityBinding(m_rLastSpawnedViewEntity);
    if (!s_View)
    {
        return;
    }

    if (auto s_voPositionOffset = s_View.m_vPositionOffset; s_voPositionOffset.has_value())
    {
        auto s_vPositionOffset = s_voPositionOffset.value();
        if (ImGui::DragFloat3("Position Offset", &s_vPositionOffset.x, 10.f))
        {
            s_View.m_vPositionOffset = s_vPositionOffset;
        }
    }
    else
    {
        ImGui::TextUnformatted("Position Offset: N/A");
    }

    if (auto s_voSize = s_View.m_vSize; s_voSize.has_value())
    {
        auto s_vSize = s_voSize.value();
        if (ImGui::DragFloat2("Size", &s_vSize.x, 10.f))
        {
            s_View.m_vSize = s_vSize;
        }
    }
    else
    {
        ImGui::TextUnformatted("Size: N/A");
    }

    if (auto s_voRotation = s_View.m_vRotation; s_voRotation.has_value())
    {
        auto s_vRotation = s_voRotation.value();
        if (ImGui::DragFloat3("Rotation", &s_vRotation.x, 1.f))
        {
            s_View.m_vRotation = s_vRotation;
        }
    }
    else
    {
        ImGui::TextUnformatted("Rotation: N/A");
    }

    /*
    if (auto s_boVideoVolumeOn = s_View.m_bVideoVolumeOn; s_boVideoVolumeOn.has_value())
    {
        auto s_bVideoVolumeOn = s_boVideoVolumeOn.value();
        if (ImGui::Checkbox("Video Volume On", &s_bVideoVolumeOn))
        {
            s_View.m_bVideoVolumeOn = s_bVideoVolumeOn;
        }
    }
    else
    {
        ImGui::TextUnformatted("Video Volume On: N/A");
    }
    */

    if (auto s_boVideoLoop = s_View.m_bVideoLoop; s_boVideoLoop.has_value())
    {
        auto s_bVideoLoop = s_boVideoLoop.value();
        if (ImGui::Checkbox("Video Loop", &s_bVideoLoop))
        {
            s_View.m_bVideoLoop = s_bVideoLoop;
        }
    }
    else
    {
        ImGui::TextUnformatted("Video Loop: N/A");
    }

    if (ImGui::Button("PlayVideo"))
    {
        s_View.PlayVideo();
    }

    if (ImGui::Button("StopVideo"))
    {
        s_View.StopVideo();
    }
}

SImageVideoViewEntityBinding ZHUDImageVideoViewEffectBase::SpawnImageAndVideoView()
{
    m_rLastSpawnedViewEntity = m_pViewSpawner->Spawn();
    return SImageVideoViewEntityBinding(m_rLastSpawnedViewEntity);
}
