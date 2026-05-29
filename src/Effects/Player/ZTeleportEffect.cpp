#include "ZTeleportEffect.h"

#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/Math.h"
#include "Helpers/PlayerUtils.h"

void ZTeleportEffect::Start()
{
    const ETeleportDirection s_eDirection = static_cast<ETeleportDirection>(
        Math::GetRandomNumber<int>(0, static_cast<int>(ETeleportDirection::NUMBER_OF_DIRECTIONS) - 1)
    );

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
    SMatrix s_mPlayerTransform;
    if (!Utils::GetPlayerTransform(s_mPlayerTransform))
    {
        return;
    }

    switch (p_eDirection)
    {
    case ETeleportDirection::Up: {
        s_mPlayerTransform.Trans.z += Math::GetRandomNumber(1.0f, 100.0f);
        break;
    }
    case ETeleportDirection::Forward: {
        const auto s_Forward = (-s_mPlayerTransform.Backward).Normalized();
        s_mPlayerTransform.Trans += s_Forward * Math::GetRandomNumber(1.0f, 10.0f);
        break;
    }
    default:
        break;
    }

    Utils::TeleportPlayer(s_mPlayerTransform);
}

REGISTER_CHAOS_EFFECT(ZTeleportEffect)
