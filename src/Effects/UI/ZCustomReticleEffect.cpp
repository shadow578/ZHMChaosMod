#include "ZCustomReticleEffect.h"

#include <imgui.h>
#include <Logging.h>

#include "Registry.h"
#include "Helpers/Math.h"

#include "Entity/EntityIds.h"

#define TAG "[ZCustomReticleEffect] "

void ZCustomReticleEffect::LoadResources()
{
    ZHUDImageVideoViewEffectBase::LoadResources();

#define ADD_IMAGE(PATH, ALLOW_H_FLIP, ALLOW_V_FLIP)                                                      \
    if (auto s_pResource = ZResourceProvider::Create<PATH>(); s_pResource && s_pResource->IsAvailable()) \
    {                                                                                                    \
        Logger::Debug(TAG "Loaded resource: " PATH);                                                     \
        SReticleImageItem s_Item = {std::move(s_pResource), ALLOW_H_FLIP, ALLOW_V_FLIP};                 \
        m_aReticleImages.emplace_back(s_Item);                                                           \
    }                                                                                                    \
    else                                                                                                 \
    {                                                                                                    \
        Logger::Warn(TAG "Failed to load resource: " PATH);                                              \
    }

    // NOTE: images must be in chunk0 for this to work
    // NOTE: this assumes images are 1920x1080.
    // NOTE: second and third parameter specifies whether the image may be flipped horizontally / vertically.
    if (HasCompanionMod(CompanionModUtil::SVersion(1, 4, 0)))
    {
        ADD_IMAGE("[assembly:/_pro/chaosmod/custom_reticle/gura.png].pc_gfx", true, false);
    }

#undef ADD_IMAGE
}

void ZCustomReticleEffect::OnEnterScene()
{
    m_ReticleView = SpawnImageAndVideoView();
    m_OriginalReticle = Utils::ZEntityFinder()
                            .EntityID(EntityId::HM3::GameEssentials::HUDReticle)
                            .FindFirst();

    m_bTrackReticle = false;
}

void ZCustomReticleEffect::OnClearScene()
{
    ZHUDImageVideoViewEffectBase::OnClearScene();
    m_aReticleImages.clear();
    m_ReticleView = {};
    m_bTrackReticle = false;
}

bool ZCustomReticleEffect::Available() const
{
    return ZHUDImageVideoViewEffectBase::Available()
           && !m_aReticleImages.empty()
           && m_ReticleView;
}

void ZCustomReticleEffect::OnDrawDebugUI()
{
    int i = 0;
    for (const auto& s_Item : m_aReticleImages)
    {
        const auto& s_pResource = s_Item.m_pResource;

        if (ImGui::Button(("SET##" + std::to_string(i)).c_str()))
        {
            m_ReticleView.m_ridImage = s_pResource->GetResourceID();
        }

        ImGui::SameLine();
        std::string s_sFlipInfo = "";
        if (s_Item.m_bAllowFlipHorizontal)
        {
            s_sFlipInfo += "H";
        }
        if (s_Item.m_bAllowFlipVertical)
        {
            s_sFlipInfo += "V";
        }
        ImGui::TextUnformatted(s_sFlipInfo.c_str());

        ImGui::SameLine();
        ImGui::TextUnformatted(fmt::format("{}", s_pResource->ToString()).c_str());

        i++;
    }

    ImGui::BeginDisabled();
    ImGui::Checkbox("Track Reticle", &m_bTrackReticle);
    ImGui::EndDisabled();

    if (ImGui::Button("Flip Horizontal"))
    {
        if (auto s_vSize = m_ReticleView.m_vSize; s_vSize.has_value())
        {
            m_ReticleView.m_vSize = SVector2(-s_vSize->x, s_vSize->y);
        }
    }

    if (ImGui::Button("Flip Vertical"))
    {
        if (auto s_vSize = m_ReticleView.m_vSize; s_vSize.has_value())
        {
            m_ReticleView.m_vSize = SVector2(s_vSize->x, -s_vSize->y);
        }
    }

    ImGui::SeparatorText("ZHUDImageVideoViewEffectBase");
    ZHUDImageVideoViewEffectBase::OnDrawDebugUI();
}

void ZCustomReticleEffect::Start()
{
    const auto s_Reticle = Math::SelectRandomElement(m_aReticleImages);
    const auto& s_pImageResource = s_Reticle.m_pResource;
    if (!s_pImageResource || !s_pImageResource->IsAvailable())
    {
        Logger::Error(TAG "Failed to set reticle image!");
        return;
    }

    m_ReticleView.m_ridImage = s_pImageResource->GetResourceID();

    auto s_vSize = SVector2(1.f, 1.f);
    if (s_Reticle.m_bAllowFlipHorizontal && Math::GetRandomBool(.5f))
    {
        s_vSize.x *= -1.f;
    }
    if (s_Reticle.m_bAllowFlipVertical && Math::GetRandomBool(.5f))
    {
        s_vSize.y *= -1.f;
    }
    m_ReticleView.m_vSize = s_vSize;

    m_ReticleView.m_vPositionOffset = SVector3(0.f, 0.f, 0.f);
    m_ReticleView.m_vRotation = SVector3(0.f, 0.f, 0.f);

    m_ReticleView.m_bVideoIsVisible = false;

    // sets image visible for us
    m_bTrackReticle = true;
}

void ZCustomReticleEffect::Stop()
{
    m_bTrackReticle = false;
    m_ReticleView.m_bImageIsVisible = false;
}

void ZCustomReticleEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (m_bTrackReticle && m_ReticleView && m_OriginalReticle)
    {
        m_ReticleView.m_bImageIsVisible = m_OriginalReticle.m_bIsVisible.value_or(false);
        m_ReticleView.m_vPositionOffset = m_OriginalReticle.m_vPositionOffset.value_or(SVector3(0.f, 0.f, 0.f));
    }
}

REGISTER_CHAOS_EFFECT(ZCustomReticleEffect);
