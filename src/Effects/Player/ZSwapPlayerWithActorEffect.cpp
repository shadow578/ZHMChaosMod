#include "ZSwapPlayerWithActorEffect.h"

#include "Glacier/ZSpatialEntity.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZSwapPlayerWithActorEffect] "

void ZSwapPlayerWithActorEffect::Start()
{
    auto s_Actor = Utils::GetRandomActor(false);
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Actor || !s_Player.m_ref)
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
    auto s_PlayerWM = s_PlayerSpatialEntity->GetObjectToWorldMatrix();

    s_ActorSpatialEntity->SetObjectToWorldMatrixFromEditor(s_PlayerWM);
    s_PlayerSpatialEntity->SetObjectToWorldMatrixFromEditor(s_ActorWM);

    Logger::Info(TAG "Swapped player with actor '{}'", s_Actor->m_sActorName);

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
