#include "ZPlayerRagdollEffects.h"
#include "IPluginInterface.h"
#include "Functions.h"

#include <Glacier/ZHitman5.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"

void ZPlayerRagdollWithImpulseEffect::Start()
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return;
    }

    ZPlayerRagdollEffectBase::Start();

    const auto s_HitmanSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>();
    auto s_WM = s_HitmanSpatial->GetWorldMatrix();

    Functions::ZRagdollHandler_ApplyImpulseOnRagdoll->Call(
        s_Player.m_pInterfaceRef->m_pRagdollHandler,
        {},
        (-s_WM.Backward * 220.f) + (s_WM.Up * 550.f),
        1,
        false
    );

    m_VelocityTracker.Reset();
    m_bActive = true;
}

void ZPlayerRagdollWithImpulseEffect::Stop()
{
    ZPlayerRagdollEffectBase::Stop();

    m_VelocityTracker.Reset();
    m_bActive = false;
}

void ZPlayerRagdollWithImpulseEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!m_bActive)
    {
        return;
    }

    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return;
    }

    m_VelocityTracker.Update(p_UpdateEvent, s_Player.m_ref);

    if (m_VelocityTracker.IsVelocityValid() && m_VelocityTracker.GetVelocity() <= 0.5f)
    {
        Stop();
    }
}

void ZPlayerRagdollWithImpulseEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Velocity: {:.2f}{}", 
        m_VelocityTracker.GetVelocity(),
        m_VelocityTracker.IsVelocityValid() ? "" : " (invalid)"
    ).c_str());
}

REGISTER_CHAOS_EFFECT(ZPlayerSimpleRagdollEffect)
REGISTER_CHAOS_EFFECT(ZPlayerPoweredRagdollEffect)
REGISTER_CHAOS_EFFECT(ZPlayerRagdollWithImpulseEffect)
