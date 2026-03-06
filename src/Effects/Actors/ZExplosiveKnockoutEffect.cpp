#include "ZExplosiveKnockoutEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include <Logging.h>

#include "Registry.h"

#define TAG "[ZExplosiveKnockoutEffect] "

constexpr float32 c_fDeathExplosionStrength = 500.0f;
constexpr float32 c_fPacifyExplosionStrength = 50.0f;

void ZExplosiveKnockoutEffect::LoadResources()
{
    ZExplosionEffectBase::LoadResources();
}

void ZExplosiveKnockoutEffect::OnClearScene()
{
    ZExplosionEffectBase::OnClearScene();
}

void ZExplosiveKnockoutEffect::OnDrawDebugUI()
{
    ZExplosionEffectBase::OnDrawDebugUI();
}

bool ZExplosiveKnockoutEffect::Available() const
{
    return ZExplosionEffectBase::Available();
}

void ZExplosiveKnockoutEffect::Start()
{
    ZActorWellbeingChangeEffectBase::Start();
}

void ZExplosiveKnockoutEffect::Stop()
{
    ZActorWellbeingChangeEffectBase::Stop();
}

void ZExplosiveKnockoutEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    ZActorWellbeingChangeEffectBase::OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);
}

void ZExplosiveKnockoutEffect::OnActorWellbeingChanged(TEntityRef<ZActor> p_rActor, const SActorState& p_OldState, const SActorState& p_NewState)
{
    if (!p_OldState.m_bDead && p_NewState.m_bDead)
    {
        Logger::Debug(TAG "Actor '{}' died", p_rActor.m_pInterfaceRef->m_sActorName.c_str());
        SpawnExplosionAtActor(p_rActor, c_fDeathExplosionStrength);
        return;
    }

    if (!p_OldState.m_bPacified && p_NewState.m_bPacified)
    {
        Logger::Debug(TAG "Actor '{}' pacified", p_rActor.m_pInterfaceRef->m_sActorName.c_str());
        SpawnExplosionAtActor(p_rActor, c_fPacifyExplosionStrength);
        return;
    }
}

void ZExplosiveKnockoutEffect::SpawnExplosionAtActor(TEntityRef<ZActor> p_rActor, const float32 p_fStrength)
{
    const auto* s_pActorSpatial = p_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_pActorSpatial)
    {
        return;
    }

    // offset explosion a bit upwards so it doesn't clip into the ground
    auto s_ActorWM = s_pActorSpatial->GetObjectToWorldMatrix();
    s_ActorWM.Trans.z += 0.25f;

    const SExplosionParams s_Params{
        .m_Position = s_ActorWM,
        .m_fTargetStrength = p_fStrength,
    };
    SpawnExplosion(s_Params);
}

REGISTER_CHAOS_EFFECT(ZExplosiveKnockoutEffect)
