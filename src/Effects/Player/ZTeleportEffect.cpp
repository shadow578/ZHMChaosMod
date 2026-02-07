#include "ZTeleportEffect.h"

#include "Glacier/ZSpatialEntity.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"

void ZTeleportEffect::Start()
{
    const ETeleportDirection s_eDirection = static_cast<ETeleportDirection>(
        Math::GetRandomNumber<int>(0, static_cast<int>(ETeleportDirection::NUMBER_OF_DIRECTIONS) - 1));

    DoTeleport(s_eDirection);
}

void ZTeleportEffect::OnDrawDebugUI()
{
    if (ImGui::Button("Teleport Up"))
    {
        DoTeleport(ETeleportDirection::Up);
    }

    if (ImGui::Button("Teleport Forward"))
    {
        DoTeleport(ETeleportDirection::Forward);
    }
}

void ZTeleportEffect::DoTeleport(const ETeleportDirection p_eDirection)
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player.m_entityRef)
    {
        return;
    }

    auto s_PlayerSpatialEntity = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_PlayerSpatialEntity)
    {
        return;
    }

    auto s_WM = s_PlayerSpatialEntity->GetObjectToWorldMatrix();

    switch (p_eDirection)
    {
    case ETeleportDirection::Up:
    {
        s_WM.Trans.z += Math::GetRandomNumber(1.0f, 100.0f);
        break;
    }
    case ETeleportDirection::Forward:
    {
        const auto s_Forward = (-s_WM.Backward).Normalized();
        s_WM.Trans += s_Forward * Math::GetRandomNumber(1.0f, 10.0f);
        break;
    }
    default:
        break;
    }

    s_PlayerSpatialEntity->SetObjectToWorldMatrixFromEditor(s_WM);
}

REGISTER_CHAOS_EFFECT(ZTeleportEffect)
