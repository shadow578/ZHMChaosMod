#include "ZHUDImageVideoViewEffectBase.h"

#include <imgui.h>

#include "Helpers/Utils.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

void ZHUDImageVideoViewEffectBase::OnEnterScene()
{
    m_rImageVideoView = Utils::ZEntityFinder()
                            .EntityID(EntityId::CompanionMod::GameEssentials::HUDImageVideoView)
                            .FindFirst();
}

void ZHUDImageVideoViewEffectBase::OnClearScene()
{
    m_rImageVideoView = {};
}

bool ZHUDImageVideoViewEffectBase::Available() const
{
    return ZCompanionModDependentEffectBase::Available()
           && m_rImageVideoView;
}

bool ZHUDImageVideoViewEffectBase::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return ZCompanionModDependentEffectBase::IsCompatibleWith(p_pOther)
           // only one video / image view is available, so only one effect at a time
           && !Utils::IsInstanceOf<ZHUDImageVideoViewEffectBase>(p_pOther);
}

void ZHUDImageVideoViewEffectBase::OnDrawDebugUI()
{
    auto s_View = GetImageAndVideoViewBinding();
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

SImageVideoViewEntityBinding ZHUDImageVideoViewEffectBase::GetImageAndVideoViewBinding() const
{
    return SImageVideoViewEntityBinding(m_rImageVideoView);
}
