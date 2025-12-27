#include "ZFlipCameraEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"

void ZFlipCameraEffect::Start()
{
    ZCameraEffectBase::Start();
    ZInterpolatingEffectBase::Start();
}

void ZFlipCameraEffect::Stop()
{
    ZCameraEffectBase::Stop();
    ZInterpolatingEffectBase::Stop();
}

void ZFlipCameraEffect::OnClearScene()
{
    ZCameraEffectBase::OnClearScene();
    ZInterpolatingEffectBase::OnClearScene();
}

void ZFlipCameraEffect::OnDrawDebugUI()
{
    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZInterpolatingEffectBase");
    ZInterpolatingEffectBase::OnDrawDebugUI();
}

void ZFlipCameraEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
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
    const auto s_OriginalWM = s_OriginalCameraSpatialEntity->GetWorldMatrix();
    auto s_TargetWM = s_OriginalWM;

    // rotate around forward axis by 180 degrees to flip the screen
    const auto s_RotationMatrix = SMatrix::RotationAxisAngle(
        (-s_TargetWM.Backward).Normalized(),
        3.14159f // 180 degrees
    );
    s_TargetWM = s_RotationMatrix * s_TargetWM;

    // interpolation
    s_TargetWM = Utils::InterpolateAffine(s_OriginalWM, s_TargetWM, GetInterpolationPoint());

    s_CameraSpatialEntity->SetWorldMatrix(s_TargetWM);
}

REGISTER_CHAOS_EFFECT(ZFlipCameraEffect)