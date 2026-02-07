#include "ZFixPlayerPositionUnlocker.h"

#include "Glacier/ZSpatialEntity.h"

#include "IPluginInterface.h"

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/EntityUtils.h"

void ZFixPlayerPositionUnlocker::Start()
{
    const auto& s_StartingLocation = Utils::ZEntityFinder()
		.EntityType("ZHeroSpawn")
		.FindFirst();

    if (!s_StartingLocation)
    {
        return;
	}

	const auto s_rPosition = Utils::GetProperty<TEntityRef<ZSpatialEntity>>(s_StartingLocation, "m_rPosition");
    if (!s_rPosition)
    {
        return;
    }

    if (const auto s_Player = SDK()->GetLocalPlayer())
    {
        if (const auto s_SpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
        {
            s_SpatialEntity->SetObjectToWorldMatrixFromEditor(
                s_rPosition.value().m_pInterfaceRef->GetObjectToWorldMatrix()
            );
        }
    }


    /*
    auto s_Actor = Utils::GetRandomActor(true);
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Actor || !s_Player)
    {
        return;
    }

    ZEntityRef s_ActorRef;
    s_Actor->GetID(s_ActorRef);
    auto s_ActorSpatialEntity = s_ActorRef.QueryInterface<ZSpatialEntity>();

    auto s_PlayerSpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>();

    if (!s_ActorSpatialEntity || !s_PlayerSpatialEntity)
    {
        return;
    }

    auto s_ActorWM = s_ActorSpatialEntity->GetObjectToWorldMatrix();
    s_PlayerSpatialEntity->SetObjectToWorldMatrixFromEditor(s_ActorWM);
    */
}

REGISTER_CHAOS_UNLOCKER(ZFixPlayerPositionUnlocker)
