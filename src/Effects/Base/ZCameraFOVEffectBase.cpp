#include "ZCameraFOVEffectBase.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/Math.h"
#include "Helpers/EntityUtils.h"

const std::string c_CameraFOVPropertyName = "m_fFovYDeg";

void ZCameraFOVEffectBase::Start()
{
    ZCameraEffectBase::Start();
    ZInterpolatingEffectBase::Start();

    m_fOriginalFOV = -1.0f;
}

void ZCameraFOVEffectBase::Stop()
{
    ZCameraEffectBase::Stop();
    ZInterpolatingEffectBase::Stop();
}

void ZCameraFOVEffectBase::OnClearScene()
{
    ZCameraEffectBase::OnClearScene();
    ZInterpolatingEffectBase::OnClearScene();
}

void ZCameraFOVEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Target FOV: {:.2f}", m_fTargetFOV).c_str());

    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZInterpolatingEffectBase");
    ZInterpolatingEffectBase::OnDrawDebugUI();
}

void ZCameraFOVEffectBase::OnFrameUpdate(const SGameUpdateEvent &p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    ZInterpolatingEffectBase::OnFrameUpdate(p_UpdateEvent, p_fEffectTimeRemaining);

    if (!IsEffectCameraActive())
    {
        return;
    }

    // get references to all involved spatial entities
    auto s_OriginalCameraSpatialEntity = GetOriginalCameraEntity().QueryInterface<ZSpatialEntity>();
    auto s_CameraSpatialEntity = GetEffectCameraEntity().QueryInterface<ZSpatialEntity>();
    if (!s_OriginalCameraSpatialEntity || !s_CameraSpatialEntity)
    {
        Stop();
        return;
    }

    // copy stock player camera
    const auto s_OriginalWM = s_OriginalCameraSpatialEntity->GetObjectToWorldMatrix();
    s_CameraSpatialEntity->SetObjectToWorldMatrixFromEditor(s_OriginalWM);

    // get original FOV once for interpolation
    if (m_fOriginalFOV <= 0.0f)
    {
        m_fOriginalFOV = Utils::GetProperty<float32>(GetOriginalCameraEntity(), c_CameraFOVPropertyName).value_or(80.0f);
    }

    auto s_fTargetFOV = m_fTargetFOV;

    // interpolation
    s_fTargetFOV = Math::Interpolate(m_fOriginalFOV, s_fTargetFOV, GetInterpolationPoint());

    auto s_rEffectCamera = GetEffectCameraEntity();
    Utils::SetProperty<float32>(s_rEffectCamera, c_CameraFOVPropertyName, s_fTargetFOV);
}
