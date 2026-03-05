#include "ZExplodeNearbyActorsEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/ActorUtils.h"

#define TAG "[ZExplodeNearbyActorsEffect] "

void ZExplodeNearbyActorsEffect::Start()
{
    // get player pos
    const auto s_rPlayer = SDK()->GetLocalPlayer();
    if (!s_rPlayer)
    {
        return;
    }

    const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_pPlayerSpatial)
    {
        return;
    }

    const auto s_mPlayerTrans = s_pPlayerSpatial->GetObjectToWorldMatrix();

    // spawn explosion at all nearby actors
    for (const auto& s_rActor : Utils::GetActors(false, false))
    {
        const auto* s_pActorSpatial = s_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();
        if (!s_pActorSpatial)
        {
            continue;
        }

        auto s_mActorTrans = s_pActorSpatial->GetObjectToWorldMatrix();
        const auto s_fDistance = float4::Distance(s_mPlayerTrans.Pos, s_mActorTrans.Pos);
        if (s_fDistance > m_fRadius)
        {
            continue;
        }

        // offset explosion a bit upwards so it doesn't clip into the ground
        s_mActorTrans.Trans.z += 0.25f;

        SExplosionParams s_Params{
            .m_Position = s_mActorTrans,

            // slow down a bit to reduce stress on engine
            .m_fFuseTimeMin = 0.5f,
            .m_fFuseTimeMax = 1.5f,

            // don't blow up too hard, to reduce stress on engine
            .m_fTargetStrength = m_fTargetStrength,
        };
        SpawnExplosion(s_Params);
    }
}

void ZExplodeNearbyActorsEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_fRadius = p_pConfiguration->GetDouble("Radius", m_fRadius);
    m_fTargetStrength = p_pConfiguration->GetDouble("TargetStrength", m_fTargetStrength);
}

void ZExplodeNearbyActorsEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGui::DragFloat("Radius", &m_fRadius, 1.f, 5.f, 50.f))
    {
        p_pConfiguration->SetDouble("Radius", m_fRadius);
    }

    if (ImGui::DragFloat("Target Strength", &m_fTargetStrength, 1.f, 2.f, 100.f))
    {
        p_pConfiguration->SetDouble("TargetStrength", m_fTargetStrength);
    }
}

REGISTER_CHAOS_EFFECT(ZExplodeNearbyActorsEffect)
