#include "ZSpawnRandomItemEffect.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"

void ZSpawnRandomItemEffect::Start()
{
    if (const auto s_Player = SDK()->GetLocalPlayer())
    {
        if (const auto s_SpatialEntity = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>())
        {
            auto s_WM = s_SpatialEntity->GetObjectToWorldMatrix();

            // spawn a bit above and in front of the player
            const auto s_Forward = (-s_WM.Backward).Normalized();
            const auto s_Right = s_WM.Right.Normalized();
            const auto s_Up = s_WM.Up.Normalized();
            s_WM.Trans += s_Forward * 1.2f + s_Up * 1.0f;

			const auto& s_aRepositoryProps = GetRepositoryProps();
            for (int i = 0; i < m_pCount; i++)
            {
                // vary spawn position a bit
                s_WM.Trans += s_Right * Math::GetRandomNumber(-0.2f, 0.2f) + s_Forward * Math::GetRandomNumber(-0.2f, 0.2f) + s_Up * Math::GetRandomNumber(-0.2f, 0.2f);

                const auto s_Item = Math::SelectRandomElement(s_aRepositoryProps);
                SpawnRepositoryPropAt(s_Item, s_WM.Trans );
            }
        }
    }
}

REGISTER_CHAOS_EFFECT_PARAM(CNT1, ZSpawnRandomItemEffect, "Loot Box", 1);
REGISTER_CHAOS_EFFECT_PARAM(CNT2, ZSpawnRandomItemEffect, "Deluxe Loot Box", 5);
REGISTER_CHAOS_EFFECT_PARAM(CNT3, ZSpawnRandomItemEffect, "Super Deluxe Loot Box", 10);
