#include "ZActorsDanceEffect.h"

#include <Logging.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZActorsDanceEffect] "

void ZActorsDanceEffect::Start()
{
    m_bUseLambicDance = Math::GetRandomBool(0.5f);

    SetAllActorsDancing(true);
}

void ZActorsDanceEffect::Stop()
{
    SetAllActorsDancing(false);
}

void ZActorsDanceEffect::SetAllActorsDancing(const bool p_bDancing)
{
    for (const auto& s_rActor : Utils::GetActors(false, false))
    {
        const auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(s_rActor.m_entityRef);
        if (!s_rActorSpatial)
        {
            continue;
        }

        if (m_bUseLambicDance)
        {
            // Act_MR_Lambic_Dance:
            if (auto s_Helper = GetLambicDanceBinding(s_rActor))
            {
                if (p_bDancing)
                {
                    if (auto s_rActSpatial = s_Helper.QuerySpatial())
                    {
                        // move target spatial to actor position
                        s_rActSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_rActorSpatial.m_pInterfaceRef->GetObjectToWorldMatrix());
                        
                        // start act, without snapping to pos
                        s_Helper.m_MovementType = ZActBehaviorEntity_EMovementType::MT_WALK;
                        s_Helper.Start();
                    }
                }
                else
                {
                    s_Helper.Cancel();
                }
            }
        }
        else
        {
            // Act_MR_Stand_Dance_Mat:
            if (auto s_Helper = GetStandDanceMatBinding(s_rActor))
            {
                if (p_bDancing)
                {
                    if (auto s_rActSpatial = s_Helper.QuerySpatial())
                    {
                        // move target spatial to actor position
                        s_rActSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_rActorSpatial.m_pInterfaceRef->GetObjectToWorldMatrix());

                        // randomly set expert mode for 20% of actors
                        s_Helper.m_bExpertMode = Math::GetRandomBool(0.2f);

                        // start act, without snapping to pos
                        s_Helper.m_MovementType = ZActBehaviorEntity_EMovementType::MT_WALK;
                        s_Helper.Start();
                    }
                }
                else
                {
                    s_Helper.Cancel();
                }
            }
        }
    }
}

REGISTER_CHAOS_EFFECT(ZActorsDanceEffect);
