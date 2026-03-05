#include "ZCameraWaveFOVEffect.h"

#include <cmath>

#include <Glacier/SGameUpdateEvent.h>
#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"

const std::string c_sCameraFOVPropertyName = "m_fFovYDeg";

void ZCameraWaveFOVEffect::Start()
{
    ZCameraEffectBase::Start();
    m_fTimeSinceStart = 0.0f;
}

void ZCameraWaveFOVEffect::OnDrawDebugUI()
{
    ImGui::DragFloat("Frequency", &m_fFrequency, 0.01f, 0.01f, 1.0f);
    ImGui::DragFloat("Min FOV", &m_fMinFOV, 1.0f, 1.0f, m_fMaxFOV - 1.0f);
    ImGui::DragFloat("Max FOV", &m_fMaxFOV, 1.0f, m_fMinFOV + 1.0f, 179.0f);

    ImGui::TextUnformatted(fmt::format("Time Since Start(): {:.2f}", m_fTimeSinceStart).c_str());
    ImGui::TextUnformatted(fmt::format("Current Target FOV: {:.2f}", GetTargetFOV()).c_str());

    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();
}

void ZCameraWaveFOVEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!IsEffectCameraActive())
    {
        return;
    }

    // get references to spatials
    const auto* s_pOriginalCameraSpatial = GetOriginalCameraEntity().QueryInterface<ZSpatialEntity>();
    auto* s_pCameraSpatial = GetEffectCameraEntity().QueryInterface<ZSpatialEntity>();
    if (!s_pOriginalCameraSpatial || !s_pCameraSpatial)
    {
        Stop();
        return;
    }

    // copy stock camera spatial to effect cam
    s_pCameraSpatial->SetObjectToWorldMatrixFromEditor(s_pOriginalCameraSpatial->GetObjectToWorldMatrix());

    // apply FOV
    auto s_rEffectCamera = GetEffectCameraEntity();
    Utils::SetProperty<float32>(s_rEffectCamera, c_sCameraFOVPropertyName, GetTargetFOV());

    // advance time
    m_fTimeSinceStart += p_UpdateEvent.m_GameTimeDelta.ToSeconds();
}

float32 ZCameraWaveFOVEffect::GetTargetFOV() const
{
    const float32 s_fCenterFOV = 0.5f * (m_fMinFOV + m_fMaxFOV);
    const float32 s_fAmplitude = 0.5f * (m_fMaxFOV - m_fMinFOV);

    constexpr float32 s_fTwoPi = 6.2831853071795864769f;
    const float32 fWave = std::sin(s_fTwoPi * m_fFrequency * m_fTimeSinceStart);

    float32 fTargetFOV = s_fCenterFOV + (s_fAmplitude * fWave);
    fTargetFOV = std::clamp(fTargetFOV, m_fMinFOV, m_fMaxFOV);

    return fTargetFOV;
}

REGISTER_CHAOS_EFFECT(ZCameraWaveFOVEffect);
