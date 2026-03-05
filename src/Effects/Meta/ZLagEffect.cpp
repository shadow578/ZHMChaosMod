#include "ZLagEffect.h"

#include <Glacier/SGameUpdateEvent.h>

#include "EffectRegistry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/ImGuiExtras.h"

void ZLagEffect::Start()
{
    m_bEnabled = true;
}

void ZLagEffect::Stop()
{
    m_bEnabled = false;
}

void ZLagEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!m_bEnabled)
    {
        return;
    }

    const float32 s_fTargetFrameTime = 1.0f / m_fTargetFPS;
    const float32 s_fTimeToWaste = s_fTargetFrameTime - p_UpdateEvent.m_RealTimeDelta.ToSeconds();
    if (s_fTimeToWaste <= 0.0f)
    {
        return;
    }

    const DWORD s_nTimeToWaste = static_cast<DWORD>(s_fTimeToWaste * 1000.0f);
    const DWORD s_nStartTime = GetTickCount();
    volatile float64 s_fDummy = 0.0;
    while ((GetTickCount() - s_nStartTime) < s_nTimeToWaste)
    {
        // inner loop to prevent too many calls to GetTickCount
        for (int i = 0; i < 10000; i++)
        {
            s_fDummy += std::sin(s_fDummy) * std::cos(s_fDummy);
        }
    }
}

void ZLagEffect::OnDrawDebugUI()
{
    ImGui::DragFloat("Target FPS", &m_fTargetFPS, 1.0f, 30.0f);
}

void ZLagEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_fTargetFPS = p_pConfiguration->GetDouble("TargetFPS", m_fTargetFPS);
}

void ZLagEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGui::DragFloat("Target FPS", &m_fTargetFPS, 1.f, 5.f, 30.f))
    {
        p_pConfiguration->SetDouble("TargetFPS", m_fTargetFPS);
    }
}

REGISTER_CHAOS_EFFECT_PARAM(FPS5, ZLagEffect, 5.0f)
