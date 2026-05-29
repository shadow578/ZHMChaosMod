#include "ZLargeMinimapEffect.h"

#include <imgui.h>
#include <Logging.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/Utils.h"
#include "Helpers/EntityUtils.h"

#include "Entity/EntityIds.h"
#include "Entity/Bindings/SZUIControlEntityBinding.h"

#include "Effects/UI/ZNoHUDEffect.h"

#define TAG "[ZLargeMinimapEffect] "

void ZLargeMinimapEffect::OnEnterScene()
{
    m_rMinimapContainer = Utils::ZEntityFinder()
                              .EntityID(EntityId::HM3::Minimap::Container)
                              .FindFirst();
    m_rMinimapHUDStatusMarkers = Utils::ZEntityFinder()
                                     .EntityID(EntityId::HM3::Minimap::HUDStatusMarkers)
                                     .FindFirst();
}

void ZLargeMinimapEffect::OnClearScene()
{
    m_rMinimapContainer = {};
    m_rMinimapHUDStatusMarkers = {};
}

bool ZLargeMinimapEffect::Available() const
{
    return IChaosEffect::Available()
           && m_rMinimapContainer
           && m_rMinimapHUDStatusMarkers;
}

bool ZLargeMinimapEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           // joke doesn't really work when HUD is not visible...
           && !Utils::IsInstanceOf<ZNoHUDEffect>(p_pOther);
}

void ZLargeMinimapEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_bReduceAlpha = p_pConfiguration->GetBool("ReduceAlpha", false);
}

void ZLargeMinimapEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGui::Checkbox("Reduce Minimap Opacity", &m_bReduceAlpha))
    {
        p_pConfiguration->SetBool("ReduceAlpha", m_bReduceAlpha);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Reduce the opacity of the minimap when active, so you don't have to go \"i can't see half my screen\", as if that wasn't the point of this.");
    }
}

void ZLargeMinimapEffect::Start()
{
    SetMaximap(true);
}

void ZLargeMinimapEffect::Stop()
{
    SetMaximap(false);
}

void ZLargeMinimapEffect::SetMaximap(const bool p_bEnabled)
{
    auto s_ContainerBinding = SZUIControlEntityBinding(m_rMinimapContainer);
    auto s_HUDStatusMarkersBinding = SZUIControlEntityBinding(m_rMinimapHUDStatusMarkers);
    if (!s_ContainerBinding || !s_HUDStatusMarkersBinding)
    {
        Logger::Debug(TAG "Failed to setup bindings");
        return;
    }

    if (p_bEnabled)
    {
        // set maximap configuration
        // to find these, you'll have to test manually.
        // it seems to me that IOI's doing some weird offset in a offset kinda thing, but idk
        // i didn't look to much into it
        s_ContainerBinding.m_vSize = SVector2(75.f, 75.f);
        s_ContainerBinding.m_eAlignmentType = ZUIControlLayoutLegacyAspect_EAlignmentType::E_ALIGNMENT_TYPE_Center;
        s_ContainerBinding.m_vPositionOffset = SVector3(405.f, 405.f, 0.f);

        s_HUDStatusMarkersBinding.m_vSize = SVector2(5.8f, 5.8f);

        if (m_bReduceAlpha)
        {
            s_ContainerBinding.m_fAlpha = 50.f;
        }

        Logger::Debug(TAG "Minimap configuration set to MAXIMAP, can you see better now?");
    }
    else
    {
        // restore default configuration, as set in minimap TEMP
        s_ContainerBinding.m_vSize = SVector2(36.f, 36.f);
        s_ContainerBinding.m_eAlignmentType = ZUIControlLayoutLegacyAspect_EAlignmentType::E_ALIGNMENT_TYPE_BottomLeft;
        s_ContainerBinding.m_vPositionOffset = SVector3(419.f, -16.f, 0.f);

        s_HUDStatusMarkersBinding.m_vSize = SVector2(2.778f, 2.778f); // rounded

        s_ContainerBinding.m_fAlpha = 100.f;

        Logger::Debug(TAG "Minimap configuration restored to default");
    }
}

REGISTER_CHAOS_EFFECT(ZLargeMinimapEffect);
