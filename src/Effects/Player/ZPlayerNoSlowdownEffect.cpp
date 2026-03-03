#include "ZPlayerNoSlowdownEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/Math.h"

#define TAG "[ZPlayerNoSlowdownEffect] "

void ZPlayerNoSlowdownEffect::Start()
{
    m_eState = EState::ActivationGrace;
    m_fGraceTimeRemaining = m_fActivationGraceTime;
}

void ZPlayerNoSlowdownEffect::Stop()
{
    m_eState = EState::Inactive;
}

void ZPlayerNoSlowdownEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (m_eState == EState::Inactive)
    {
        return;
    }

    if (const auto s_rPlayer = SDK()->GetLocalPlayer())
    {
        m_VelocityTracker.Update(p_UpdateEvent, s_rPlayer.m_entityRef);
    }
}

void ZPlayerNoSlowdownEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    m_nUIFlash++;
    m_nUIFlash = m_nUIFlash % 2;

    switch (m_eState)
    {
    case EState::ActivationGrace: {
        m_fGraceTimeRemaining -= p_fDeltaTime;
        if (m_fGraceTimeRemaining <= 0.0f)
        {
            m_eState = EState::AtSpeed;
            m_fGraceTimeRemaining = 0.0f;
        }
        break;
    }
    case EState::AtSpeed: {
        if (IsBelowTargetVelocity())
        {
            m_eState = EState::BelowSpeedGrace;
            m_fGraceTimeRemaining = m_fBelowSpeedGraceTime;
        }
        break;
    }
    case EState::BelowSpeedGrace: {
        if (!IsBelowTargetVelocity())
        {
            m_fGraceTimeRemaining += p_fDeltaTime;
            if (m_fGraceTimeRemaining > m_fBelowSpeedGraceTime)
            {
                m_eState = EState::AtSpeed;
            }
        }
        else
        {
            m_fGraceTimeRemaining -= p_fDeltaTime;
            if (m_fGraceTimeRemaining <= 0.0f)
            {
                m_eState = EState::BelowSpeed;
                m_fGraceTimeRemaining = Math::GetRandomNumber(m_fExplosionDelayMin, m_fExplosionDelayMax);
            }
        }
        break;
    }
    case EState::BelowSpeed: {
        m_fGraceTimeRemaining -= p_fDeltaTime;
        if (m_fGraceTimeRemaining <= 0.0f)
        {
            Explode();
            m_eState = EState::Inactive;
        }
    }
    case EState::Inactive:
    default:
        break;
    }
}

void ZPlayerNoSlowdownEffect::OnDrawUI(const bool p_bHasFocus)
{
    if (m_eState == EState::Inactive)
    {
        return;
    }

    // convert in-game velocity to km/h for display.
    // usa players can suck a lemon.
    float32 s_fSpeed = 0.0f;
    if (m_VelocityTracker.IsVelocityValid())
    {
        // 2.5 = walking speed ~= 5 km/h,
        // 5.0 = sprinting speed ~= 10 km/h
        s_fSpeed = m_VelocityTracker.GetVelocity() * 2.0f;
    }

    std::string s_sText = fmt::format("{:.1f} km/h", s_fSpeed);
    if (m_eState == EState::ActivationGrace)
    {
        s_sText = fmt::format("Get Ready! {:.1f} Seconds Remaining!", m_fGraceTimeRemaining);
    }

    ImU32 s_TextColor = IM_COL32(0, 255, 0, 255); // green
    if (m_eState == EState::BelowSpeedGrace || m_eState == EState::BelowSpeed)
    {
        s_TextColor = (m_nUIFlash == 0)
                          ? IM_COL32(255, 255, 0, 255) // yellow
                          : IM_COL32(255, 0, 0, 255);  // red
    }

    ImGuiWindowFlags s_OverlayFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBackground;

    // set font size early to get correct text size for positioning
    ImGui::PushFont(SDK()->GetImGuiRegularFont(), 64.f);

    // center at the top of the screen
    const auto s_vTextSize = ImGui::CalcTextSize(s_sText.c_str());
    const auto& s_ImGuiIO = ImGui::GetIO();
    ImGui::SetNextWindowPos(
        ImVec2(
            (s_ImGuiIO.DisplaySize.x / 2.f) - (s_vTextSize.x / 2.f), // center horizontally
            (s_ImGuiIO.DisplaySize.y / 4.f) - s_vTextSize.y          // above 1/4 point
        ),
        ImGuiCond_Always
    );

    // full opacity, even without focus
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

    const auto s_OvelayShowing = ImGui::Begin("##ZPlayerNoSlowdownEffect", NULL, s_OverlayFlags);

    if (s_OvelayShowing)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, s_TextColor);
        ImGui::TextUnformatted(s_sText.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::End();
    ImGui::PopFont();
    ImGui::PopStyleVar(); // ImGuiStyleVar_Alpha
}

void ZPlayerNoSlowdownEffect::OnDrawDebugUI()
{
    ImGui::DragFloat("Target Speed", &m_fTargetVelocity, 0.01f, 0.0f, 5.0f);
    ImGui::DragFloat("Activation Grace Time", &m_fActivationGraceTime, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Below Speed Grace Time", &m_fBelowSpeedGraceTime, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Explosion Delay Min", &m_fExplosionDelayMin, 0.1f, 0.0f, 10.0f);
    ImGui::DragFloat("Explosion Delay Max", &m_fExplosionDelayMax, 0.1f, 0.0f, 10.0f);

    ImGui::TextUnformatted(fmt::format("State: {}", StateToString(m_eState)).c_str());
    ImGui::TextUnformatted(fmt::format("Velocity: {:.2f}{}", m_VelocityTracker.GetVelocity(), m_VelocityTracker.IsVelocityValid() ? "" : " (invalid)").c_str());
    ImGui::TextUnformatted(fmt::format("Grace Time Remaining: {:.2f}", m_fGraceTimeRemaining).c_str());
}

bool ZPlayerNoSlowdownEffect::IsBelowTargetVelocity() const
{
    return m_VelocityTracker.IsVelocityValid()
           && m_VelocityTracker.GetVelocity() < m_fTargetVelocity;
}

void ZPlayerNoSlowdownEffect::Explode()
{
    if (const auto s_rPlayer = SDK()->GetLocalPlayer())
    {
        if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
        {
            SExplosionParams s_Params{
                .m_Position = s_pPlayerSpatial->GetObjectToWorldMatrix(),
                .m_fTargetStrength = 500.0f, // deader than dead
            };
            SpawnExplosion(s_Params);
        }
    }
}

std::string ZPlayerNoSlowdownEffect::StateToString(const EState p_eState)
{
    switch (p_eState)
    {
    case EState::Inactive:
        return "Inactive";
    case EState::ActivationGrace:
        return "ActivationGrace";
    case EState::AtSpeed:
        return "AtSpeed";
    case EState::BelowSpeedGrace:
        return "BelowSpeedGrace";
    case EState::BelowSpeed:
        return "BelowSpeed";
    default:
        return "NA";
    }
}

REGISTER_CHAOS_EFFECT(ZPlayerNoSlowdownEffect);
