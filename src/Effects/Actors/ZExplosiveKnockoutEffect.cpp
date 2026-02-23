#include "ZExplosiveKnockoutEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "Logging.h"

#include "EffectRegistry.h"

#define TAG "[ZExplosiveKnockoutEffect] "

constexpr float32 c_fDeathExplosionStrength = 500.0f;
constexpr float32 c_fPacifyExplosionStrength = 50.0f;

void ZExplosiveKnockoutEffect::OnActorWellbeingChanged(ZActor *p_pActor, const SActorState &p_OldState, const SActorState &p_NewState)
{
    if (!p_OldState.m_bDead && p_NewState.m_bDead)
    {
        Logger::Debug(TAG "Actor '{}' died", p_pActor->m_sActorName.c_str());
        SpawnExplosionAtActor(p_pActor, c_fDeathExplosionStrength);
        return;
    }

    if (!p_OldState.m_bPacified && p_NewState.m_bPacified)
    {
        Logger::Debug(TAG "Actor '{}' pacified", p_pActor->m_sActorName.c_str());
        SpawnExplosionAtActor(p_pActor, c_fPacifyExplosionStrength);
        return;
    }
}

void ZExplosiveKnockoutEffect::SpawnExplosionAtActor(ZActor *p_pActor, const float32 p_fStrength)
{
    ZEntityRef s_Ref;
    p_pActor->GetID(s_Ref);
    if (!s_Ref)
    {
        return;
    }

    const auto *s_pActorSpatial = s_Ref.QueryInterface<ZSpatialEntity>();
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
