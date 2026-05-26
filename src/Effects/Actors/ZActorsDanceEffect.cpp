#include "ZActorsDanceEffect.h"

#include <Logging.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZActorsDanceEffect] "

void ZActorsDanceEffect::Start()
{
    const auto s_nDanceStyle = Math::GetRandomNumber<int>(0, 2);
    Logger::Info(TAG "Selected Dance Style {}", s_nDanceStyle);
    m_DanceStyle = static_cast<EDanceStyle>(s_nDanceStyle);

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

        switch (m_DanceStyle)
        {
        case EDanceStyle::Lambic: // Act_MR_Lambic_Dance
        {
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

            break;
        }
        case EDanceStyle::DanceMat: // Act_MR_Stand_Dance_Mat
        {
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

            break;
        }
        case EDanceStyle::Flamingo: // Act_MR_Stand_Mascot_Entertain
        {
            if (auto s_Helper = GetFlamingoDanceBinding(s_rActor))
            {
                if (p_bDancing)
                {
                    if (auto s_rActSpatial = s_Helper.QuerySpatial())
                    {
                        // move target spatial to actor position
                        s_rActSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_rActorSpatial.m_pInterfaceRef->GetObjectToWorldMatrix());

                        // select default mode
                        // TODO: use different modes ???
                        s_Helper.m_nMode = SFlamingoDanceActEntityBinding::MODE_DEFAULT;

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

            break;
        }
        }
    }
}

REGISTER_CHAOS_EFFECT(ZActorsDanceEffect);
