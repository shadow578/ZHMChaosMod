#include "ZLiveTvEffect.h"

#include <imgui.h>
#include <Logging.h>

#include "Registry.h"
#include "Helpers/Math.h"

#define TAG "[ZLiveTvEffect] "

void ZLiveTvEffect::LoadResources()
{
    ZHUDImageVideoViewEffectBase::LoadResources();

#define ADD_VIDEO(PATH)                                                                                  \
    if (auto s_pResource = ZResourceProvider::Create<PATH>(); s_pResource && s_pResource->IsAvailable()) \
    {                                                                                                    \
        Logger::Debug(TAG "Loaded video resource: " PATH);                                               \
        m_aVideoResources.emplace_back(std::move(s_pResource));                                          \
    }                                                                                                    \
    else                                                                                                 \
    {                                                                                                    \
        Logger::Warn(TAG "Failed to load video resource: " PATH);                                        \
    }

    // NOTE: videos must be ported to chunk0 for this to work
    ADD_VIDEO("[assembly:/_pro/effects/videos/arewestars_1minloop_640x360.avi].pc_gfxv");   // Jordan Cross music video
    ADD_VIDEO("[assembly:/_pro/effects/videos/fox_distraction_yumvideo.avi].pc_gfxv");      // Spinning Burger + Banana
    ADD_VIDEO("[assembly:/_pro/effects/videos/kandlgameplay_640x480.avi].pc_gfxv");         // Kane & Lynch Gameplay
    ADD_VIDEO("[assembly:/_pro/effects/videos/freedom_commercial_640x360.avi].pc_gfxv");    // "Dave's Guns" Commercial
    ADD_VIDEO("[assembly:/_pro/effects/videos/vermont_gnn_news_all.avi].pc_gfxv");          // News Broadcast
    ADD_VIDEO("[assembly:/_pro/effects/videos/miami_gnn_news_all.avi].pc_gfxv");            // News Broadcast
    ADD_VIDEO("[assembly:/_pro/effects/videos/baseball_pregame_loop_640x360.avi].pc_gfxv"); // Baseball pregame show
    ADD_VIDEO("[assembly:/_pro/effects/videos/dancemat_video_640x360_a.avi].pc_gfxv");      // Hokkaido Dance Mat - Perfect Performance
    // ADD_VIDEO("[assembly:/_pro/effects/videos/dancemat_video_640x360_b.avi].pc_gfxv");      // Hokkaido Dance Mat - Mid Performance

#undef ADD_VIDEO
}

void ZLiveTvEffect::OnClearScene()
{
    ZHUDImageVideoViewEffectBase::OnClearScene();
    m_aVideoResources.clear();
    m_aActiveVideoViews.clear();
}

bool ZLiveTvEffect::Available() const
{
    return ZHUDImageVideoViewEffectBase::Available()
           && !m_aVideoResources.empty();
}

void ZLiveTvEffect::OnDrawDebugUI()
{
    int i = 0;
    for (const auto& s_pResource : m_aVideoResources)
    {
        if (ImGui::Button(("PLAY##" + std::to_string(i)).c_str()))
        {
            PlayVideo(s_pResource->GetResourceID());
        }

        ImGui::SameLine();
        ImGui::TextUnformatted(fmt::format("{}", s_pResource->ToString()).c_str());

        i++;
    }

    ImGui::TextUnformatted(fmt::format("Currently Active: {}", m_aActiveVideoViews.size()).c_str());

    ImGui::SeparatorText("ZHUDImageVideoViewEffectBase");
    ZHUDImageVideoViewEffectBase::OnDrawDebugUI();
}

void ZLiveTvEffect::Start()
{
    auto s_pVideoResource = Math::SelectRandomElement(m_aVideoResources);
    if (s_pVideoResource && s_pVideoResource->IsAvailable())
    {
        Logger::Debug(TAG "Playing video {}", s_pVideoResource->ToString());
        PlayVideo(s_pVideoResource->GetResourceID());
    }
}

void ZLiveTvEffect::Stop()
{
    for (auto s_View : m_aActiveVideoViews)
    {
        s_View.m_bVideoIsVisible = false;
        s_View.StopVideo();
    }

    m_aActiveVideoViews.clear();
}

void ZLiveTvEffect::PlayVideo(const ZRuntimeResourceID& p_ridVideo)
{
    auto s_View = SpawnImageAndVideoView();
    if (!s_View)
    {
        return;
    }

    s_View.m_vPositionOffset = SVector3(
        Math::GetRandomNumber<float32>(m_vPositionMin.x, m_vPositionMax.x),
        Math::GetRandomNumber<float32>(m_vPositionMin.y, m_vPositionMax.y),
        0.f
    );
    s_View.m_vSize = m_vSize;
    s_View.m_vRotation = SVector3(0.f, 0.f, 0.f);

    s_View.m_ridVideo = p_ridVideo;
    s_View.m_bVideoLoop = true;

    s_View.m_bImageIsVisible = false;
    s_View.m_bVideoIsVisible = true;

    s_View.PlayVideo();

    m_aActiveVideoViews.emplace_back(s_View);
}

REGISTER_CHAOS_EFFECT(ZLiveTvEffect);
