#include "ZSwapPlayerWithActorEffect.h"

#include "Glacier/ZSpatialEntity.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZSwapPlayerWithActorEffect] "

void ZSwapPlayerWithActorEffect::Start()
{
    auto s_rActor = Utils::GetRandomActor(false);
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_rActor || !s_Player.m_entityRef)
    {
        return;
    }

    auto s_ActorSpatialEntity = s_rActor.m_entityRef.QueryInterface<ZSpatialEntity>();

    auto s_PlayerSpatialEntity = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>();

    if (!s_ActorSpatialEntity || !s_PlayerSpatialEntity)
    {
        return;
    }

    auto s_ActorWM = s_ActorSpatialEntity->GetObjectToWorldMatrix();
    auto s_PlayerWM = s_PlayerSpatialEntity->GetObjectToWorldMatrix();

    s_ActorSpatialEntity->SetObjectToWorldMatrixFromEditor(s_PlayerWM);
    s_PlayerSpatialEntity->SetObjectToWorldMatrixFromEditor(s_ActorWM);

    Logger::Info(TAG "Swapped player with actor '{}'", s_rActor.m_pInterfaceRef->m_sActorName);

    m_PreviousPlayerPosition = s_PlayerWM;
}

void ZSwapPlayerWithActorEffect::OnDrawDebugUI()
{
    if (ImGui::Button("Restore Last Player Position"))
    {
        Utils::TeleportPlayerTo(m_PreviousPlayerPosition);
    }
}

REGISTER_CHAOS_EFFECT(ZSwapPlayerWithActorEffect)
