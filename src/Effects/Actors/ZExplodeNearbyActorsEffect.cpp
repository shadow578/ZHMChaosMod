#include "ZExplodeNearbyActorsEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"

#define TAG "[ZExplodeNearbyActorsEffect] "

constexpr float32 c_fRadius = 10.0f;

void ZExplodeNearbyActorsEffect::Start()
{
    // get player pos
    const auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return;
    }

    const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>();
    if (!s_PlayerSpatial)
    {
        return;
    }

    const auto s_PlayerWM = s_PlayerSpatial->GetObjectToWorldMatrix();

    // spawn explosion at all nearby actors
    const auto s_aAllActors = Utils::GetActors(false, false);
    for (auto *s_pActor : s_aAllActors)
    {
        ZEntityRef s_Ref;
        s_pActor->GetID(s_Ref);
        if (!s_Ref)
        {
            continue;
        }

        const auto *s_pActorSpatial = s_Ref.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_ActorWM = s_pActorSpatial->GetObjectToWorldMatrix();
        const auto s_fDistance = float4::Distance(s_PlayerWM.Trans, s_ActorWM.Trans);
        if (s_fDistance > c_fRadius)
        {
            continue;
        }

        // offset explosion a bit upwards so it doesn't clip into the ground
        s_ActorWM.Trans.z += 0.25f;

        SExplosionParams s_Params{
            .m_Position = s_ActorWM,

            // slow down a bit to reduce stress on engine
            .m_fFuseTimeMin = 0.5f,
            .m_fFuseTimeMax = 1.5f,

            // don't blow up too hard, to reduce stress on engine
            .m_fTargetStrength = 10.0f,
        };
        SpawnExplosion(s_Params);
    }
}

REGISTER_CHAOS_EFFECT(ZExplodeNearbyActorsEffect)
