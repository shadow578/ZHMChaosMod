#include "ZSwapPlayerWithActorEffect.h"

#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZSwapPlayerWithActorEffect] "

void ZSwapPlayerWithActorEffect::Start()
{
    auto s_rActor = Utils::GetRandomActor(true);
    if (!s_rActor)
    {
        return;
    }

    auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(s_rActor.m_entityRef);
    if (!s_rActorSpatial)
    {
        return;
    }

    SMatrix s_mActorTransform = s_rActorSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    SMatrix s_mPlayerTransform;
    if (!Utils::TeleportPlayer(s_mActorTransform, s_mPlayerTransform))
    {
        return;
    }

    s_rActorSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mPlayerTransform);

    Logger::Info(TAG "Swapped player with actor '{}'", s_rActor.m_pInterfaceRef->m_sActorName);

    m_PreviousPlayerPosition = s_mPlayerTransform;
}

void ZSwapPlayerWithActorEffect::OnDrawDebugUI()
{
    if (ImGui::Button("Restore Last Player Position"))
    {
        Utils::TeleportPlayer(m_PreviousPlayerPosition);
    }
}

REGISTER_CHAOS_EFFECT(ZSwapPlayerWithActorEffect)
