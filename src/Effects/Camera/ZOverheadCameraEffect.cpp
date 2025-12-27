#include "ZOverheadCameraEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZCollision.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"

constexpr float32 c_fOverheadMinDistance = 2.0f; // at least 47's height
constexpr float32 c_fOverheadMaxDistance = 15.0f;

void ZOverheadCameraEffect::Start()
{
    ZCameraEffectBase::Start();
    ZInterpolatingEffectBase::Start();
}

void ZOverheadCameraEffect::Stop()
{
    ZCameraEffectBase::Stop();
    ZInterpolatingEffectBase::Stop();
}

void ZOverheadCameraEffect::OnClearScene()
{
    ZCameraEffectBase::OnClearScene();
    ZInterpolatingEffectBase::OnClearScene();
}

void ZOverheadCameraEffect::OnDrawDebugUI()
{
    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZInterpolatingEffectBase");
    ZInterpolatingEffectBase::OnDrawDebugUI();
}

void ZOverheadCameraEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    ZInterpolatingEffectBase::OnFrameUpdate(p_UpdateEvent, p_fEffectTimeRemaining);

    if (!IsEffectCameraActive())
    {
        return;
    }

    // get references to all involved spatial entities
    auto s_OriginalCameraSpatialEntity = GetOriginalCameraEntity().QueryInterface<ZSpatialEntity>();
    auto s_CameraSpatialEntity = GetEffectCameraEntity().QueryInterface<ZSpatialEntity>();

    auto s_Player = SDK()->GetLocalPlayer();
    auto s_PlayerSpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>();
    if (!s_OriginalCameraSpatialEntity || !s_CameraSpatialEntity || !s_PlayerSpatialEntity)
    {
        Stop();
        return;
    }

    const auto s_OriginalWM = s_OriginalCameraSpatialEntity->GetWorldMatrix();
    auto s_TargetWM = s_PlayerSpatialEntity->GetWorldMatrix();

    // raycast to ceiling, move camera up as far as possible
    s_TargetWM.Trans.z += c_fOverheadMinDistance;
    s_TargetWM.Trans.z = GetMaxOverheadZAt(
        s_TargetWM,
        c_fOverheadMaxDistance
    );

    // rotate facing downwards
    s_TargetWM.XAxis = float4(0.0f, 1.0f, 0.0f, 0.0f);
    s_TargetWM.YAxis = float4(-1.0f, 0.0f, -0.0f, 0.0f);
    s_TargetWM.ZAxis = float4(-0.0f, 0.0f, 1.0f, 0.0f);

    // interpolation
    s_TargetWM = Utils::InterpolateAffine(s_OriginalWM, s_TargetWM, GetInterpolationPoint());

    s_CameraSpatialEntity->SetWorldMatrix(s_TargetWM);
}

float32 ZOverheadCameraEffect::GetMaxOverheadZAt(const SMatrix& p_Position, const float32 p_fMax)
{
    if (!*Globals::CollisionManager)
    {
        return p_fMax;
    }

    ZRayQueryInput s_RayInput{
        .m_vFrom = p_Position.Trans,
        .m_vTo = p_Position.Trans + (p_Position.Up * p_fMax),
    };
    ZRayQueryOutput s_RayOutput{};

    if (!(*Globals::CollisionManager)->RayCastClosestHit(s_RayInput, &s_RayOutput))
    {
        // ray didn't hit anything
        return p_fMax;
    }

    return s_RayOutput.m_vPosition.z;
}

REGISTER_CHAOS_EFFECT(ZOverheadCameraEffect)
