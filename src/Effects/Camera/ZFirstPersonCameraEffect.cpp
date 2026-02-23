#include "ZFirstPersonCameraEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/Math.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/EntityUtils.h"

void ZFirstPersonCameraEffect::Start()
{
    const auto s_rPlayer = SDK()->GetLocalPlayer();
	if (!s_rPlayer)
    {
        return;
    }

    m_rPlayerHeadAttachEntity = Utils::GetPlayerHeadAttachEntity(s_rPlayer);
	if (!m_rPlayerHeadAttachEntity)
    {
        return;
    }

    ZCameraEffectBase::Start();
    ZInterpolatingEffectBase::Start();

    if (IsEffectCameraActive())
    {
        auto s_rEffectCamera = GetEffectCameraEntity();

        // increase near clipping to stop inside of head from rendering
        Utils::SetProperty<float32>(s_rEffectCamera, "m_fNearZ", 0.45f);
    }
}

void ZFirstPersonCameraEffect::Stop()
{
    ZCameraEffectBase::Stop();
    ZInterpolatingEffectBase::Stop();

    m_rPlayerHeadAttachEntity = {};
}

void ZFirstPersonCameraEffect::OnClearScene()
{
    ZCameraEffectBase::OnClearScene();
    ZInterpolatingEffectBase::OnClearScene();
}

void ZFirstPersonCameraEffect::OnDrawDebugUI()
{
    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZInterpolatingEffectBase");
    ZInterpolatingEffectBase::OnDrawDebugUI();
}

void ZFirstPersonCameraEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    ZInterpolatingEffectBase::OnFrameUpdate(p_UpdateEvent, p_fEffectTimeRemaining);

    if (!IsEffectCameraActive() || !m_rPlayerHeadAttachEntity)
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
    auto s_TargetWM = s_OriginalWM;

    // move camera to player head position
	s_TargetWM.Pos = m_rPlayerHeadAttachEntity.m_pInterfaceRef->GetObjectToWorldMatrix().Pos;

    // interpolation
    s_TargetWM = Math::InterpolateAffine(s_OriginalWM, s_TargetWM, GetInterpolationPoint());

    s_CameraSpatialEntity->SetObjectToWorldMatrixFromEditor(s_TargetWM);
}

REGISTER_CHAOS_EFFECT(ZFirstPersonCameraEffect)