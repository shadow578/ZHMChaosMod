#include "ZSpawnRandomItemEffect.h"

#include "Registry.h"
#include "Helpers/Math.h"
#include "Helpers/PlayerUtils.h"

void ZSpawnRandomItemEffect::Start()
{
    SMatrix s_mPlayerTransform;
    if (!Utils::GetPlayerTransform(s_mPlayerTransform))
    {
        return;
    }

    // spawn a bit above and in front of the player
    const auto s_Forward = (-s_mPlayerTransform.Backward).Normalized();
    const auto s_Right = s_mPlayerTransform.Right.Normalized();
    const auto s_Up = s_mPlayerTransform.Up.Normalized();
    s_mPlayerTransform.Trans += s_Forward * 1.2f + s_Up * 1.0f;

    const auto& s_aRepositoryProps = GetRepositoryProps();
    for (int i = 0; i < m_pCount; i++)
    {
        // vary spawn position a bit
        s_mPlayerTransform.Trans += s_Right * Math::GetRandomNumber(-0.2f, 0.2f) + s_Forward * Math::GetRandomNumber(-0.2f, 0.2f) + s_Up * Math::GetRandomNumber(-0.2f, 0.2f);

        const auto s_Item = Math::SelectRandomElement(s_aRepositoryProps);
        SpawnRepositoryPropAt(s_Item, s_mPlayerTransform.Trans);
    }
}

REGISTER_CHAOS_EFFECT_PARAM(CNT1, ZSpawnRandomItemEffect, "Loot Box", 1);
REGISTER_CHAOS_EFFECT_PARAM(CNT2, ZSpawnRandomItemEffect, "Deluxe Loot Box", 5);
REGISTER_CHAOS_EFFECT_PARAM(CNT3, ZSpawnRandomItemEffect, "Super Deluxe Loot Box", 10);
