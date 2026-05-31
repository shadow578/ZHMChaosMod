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

    m_pImageResource = ZResourceProvider::Create(m_sImageResourcePath);
    if (m_pImageResource && m_pImageResource->IsAvailable())
    {
        Logger::Debug(TAG "Loaded resource: {}", m_sImageResourcePath);
    }
    else
    {
        Logger::Warn(TAG "Failed to load resource: {}", m_sImageResourcePath);
        m_pImageResource = nullptr;
    }
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
    m_pImageResource = nullptr;
    m_ReticleView = {};
    m_OriginalReticle = {};
    m_bTrackReticle = false;
}

bool ZCustomReticleEffect::Available() const
{
    return ZHUDImageVideoViewEffectBase::Available()
           && m_pImageResource && m_pImageResource->IsAvailable()
           && m_ReticleView
           && m_OriginalReticle;
}

void ZCustomReticleEffect::OnDrawDebugUI()
{
    ImGui::Checkbox("Allow Flip Horizontal", &m_bAllowFlipHorizontal);
    ImGui::Checkbox("Allow Flip Vertical", &m_bAllowFlipVertical);

    ImGui::BeginDisabled();
    ImGui::Checkbox("Is Tracking Reticle", &m_bTrackReticle);
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
    if (!m_pImageResource || !m_pImageResource->IsAvailable())
    {
        Logger::Error(TAG "Failed to set reticle image!");
        return;
    }

    m_ReticleView.m_ridImage = m_pImageResource->GetResourceID();

    auto s_vSize = SVector2(1.f, 1.f);
    if (m_bAllowFlipHorizontal && Math::GetRandomBool(.5f))
    {
        s_vSize.x *= -1.f;
    }
    if (m_bAllowFlipVertical && Math::GetRandomBool(.5f))
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

REGISTER_CHAOS_EFFECT_PARAM(
    gura,
    ZCustomReticleEffect,
    "gura",                                                     // name
    "Gura",                                                     // display name
    "[assembly:/_pro/chaosmod/custom_reticle/gura.png].pc_gfx", // res path
    true,                                                       // allow H-flip
    false                                                       // allow V-flip
);
REGISTER_CHAOS_EFFECT_PARAM(
    spiderman,
    ZCustomReticleEffect,
    "spiderman",                                                    // name
    "Spiderman",                                                    // display name
    "[assembly:/_pro/chaosmod/custom_reticle/spiderman.png].pc_gfx" // res path
);
REGISTER_CHAOS_EFFECT_PARAM(
    leo,
    ZCustomReticleEffect,
    "leo",                                                     // name
    "Leo",                                                     // display name
    "[assembly:/_pro/chaosmod/custom_reticle/leo.png].pc_gfx", // res path
    true                                                       // allow H-flip
);
REGISTER_CHAOS_EFFECT_PARAM(
    amogus,
    ZCustomReticleEffect,
    "amogus",                                                     // name
    "Amogus",                                                     // display name
    "[assembly:/_pro/chaosmod/custom_reticle/amogus.png].pc_gfx", // res path
    true                                                          // allow H-flip
);
REGISTER_CHAOS_EFFECT_PARAM(
    clickbait,
    ZCustomReticleEffect,
    "clickbait",                                                    // name
    "Clickbait",                                                    // display name
    "[assembly:/_pro/chaosmod/custom_reticle/clickbait.png].pc_gfx" // res path
);
