#include "ZExplodeActorsOnTouchEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"

#define TAG "[ZExplodeActorsOnTouchEffect] "

constexpr float32 c_fRadius = 0.75f;

void ZExplodeActorsOnTouchEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (!m_bEnable)
    {
        return;
    }

    // get player pos
    const auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return;
    }

    const auto s_PlayerSpatial = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_PlayerSpatial)
    {
        return;
    }

    const auto s_PlayerWM = s_PlayerSpatial->GetObjectToWorldMatrix();

    // spawn explosion at all nearby actors
    m_fNearestActorDistance = 9999.0f;
    for (const auto &s_rActor : Utils::GetActors(true, true))
    {
        const auto *s_pActorSpatial = s_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_ActorWM = s_pActorSpatial->GetObjectToWorldMatrix();
        const auto s_fDistance = float4::Distance(s_PlayerWM.Trans, s_ActorWM.Trans);

        m_fNearestActorDistance = s_fDistance < m_fNearestActorDistance ? s_fDistance : m_fNearestActorDistance;
        if (s_fDistance > c_fRadius)
        {
            continue;
        }

        // disable for debug
        if (!m_bSpawnExplosions)
        {
            continue;
        }

        // offset explosion a bit upwards so it doesn't clip into the ground
        s_ActorWM.Trans.z += 0.25f;

        SExplosionParams s_Params{
            .m_Position = s_ActorWM,
            .m_fTargetStrength = 10.0f,
        };
        SpawnExplosion(s_Params);
    }
}

void ZExplodeActorsOnTouchEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Nearest Actor Distance: {}", m_fNearestActorDistance).c_str());
    ImGui::Checkbox("Spawn Explosions", &m_bSpawnExplosions);
}

REGISTER_CHAOS_EFFECT(ZExplodeActorsOnTouchEffect)
