#include "ZCameraViewBasedGravityEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/CameraUtils.h"

void ZCameraViewBasedGravityEffect::Start()
{
    m_bIsActive = true;
}

void ZCameraViewBasedGravityEffect::Stop()
{
    m_bIsActive = false;
    RestoreDefaultGravity();
}

void ZCameraViewBasedGravityEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (!m_bIsActive)
    {
        return;
    }

    ZEntityRef s_Camera;
    if (!Utils::GetActiveCamera(s_Camera))
    {
        m_bIsActive = false;
        return;
    }

    const auto s_CameraSpatial = s_Camera.QueryInterface<ZSpatialEntity>();
    if (!s_CameraSpatial)
    {
        m_bIsActive = false;
        return;
    }

    const auto s_WM = s_CameraSpatial->GetWorldMatrix();
    const auto s_Forward = (-s_WM.Backward).Normalized();

    constexpr float32 s_fGravity = -9.81f;
    const auto s_vGravity = SVector3(
        s_fGravity * s_Forward.x,
        s_fGravity * s_Forward.y,
        s_fGravity * s_Forward.z
    );

    SetGravity(s_vGravity);    
}

REGISTER_CHAOS_EFFECT(ZCameraViewBasedGravityEffect);
