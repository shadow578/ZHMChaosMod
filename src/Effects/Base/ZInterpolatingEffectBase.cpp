#include "ZInterpolatingEffectBase.h"

#include <Glacier/SGameUpdateEvent.h>


void ZInterpolatingEffectBase::Start()
{
    if (m_eState != EState::Idle)
    {
        return;
    }

    m_eState = EState::InterpolateIn;
    m_fInterpolationPoint = 0.0f;
}

void ZInterpolatingEffectBase::Stop()
{
    m_eState = EState::Idle;
}

void ZInterpolatingEffectBase::OnClearScene()
{
    m_eState = EState::Idle;
}

void ZInterpolatingEffectBase::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    switch (m_eState)
    {
    case EState::InterpolateIn:
    {
        m_fInterpolationPoint += p_UpdateEvent.m_GameTimeDelta.ToSeconds() / m_fInterpolationTime;

        if (m_fInterpolationPoint >= 1.0f)
        {
            m_fInterpolationPoint = 1.0f;
            m_eState = EState::Idle;
        }
        break;
    }
    case EState::InterpolateOut:
    {
        m_fInterpolationPoint -= p_UpdateEvent.m_GameTimeDelta.ToSeconds() / m_fInterpolationTime;

        if (m_fInterpolationPoint <= 0.0f)
        {
            m_fInterpolationPoint = 0.0f;
            m_eState = EState::Idle;
        }
        break;
    }
    default:
        break;
    }

    // lerp out early
    if (p_fEffectTimeRemaining > 0.0f && p_fEffectTimeRemaining <= (m_fInterpolationTime + 0.1f))
    {
        m_eState = EState::InterpolateOut;
    }
}

void ZInterpolatingEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("State: {}", static_cast<int>(m_eState)).c_str());
    ImGui::TextUnformatted(fmt::format("Interpolation Point: {}", m_fInterpolationPoint).c_str());
}
