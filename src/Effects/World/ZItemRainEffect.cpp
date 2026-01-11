#include "ZItemRainEffect.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"

constexpr float32 c_fSpawnHeight = 50.0f;
constexpr float32 c_fSpawnRadius = 30.0f;

void ZItemRainEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (!m_bActive)
    {
        return;
    }

    m_fTimeSinceLastSpawn += p_fDeltaTime;
    if (m_fTimeSinceLastSpawn < m_fSpawnInterval)
    {
        return;
	}
	m_fTimeSinceLastSpawn = 0.0f;

    if (const auto s_Player = SDK()->GetLocalPlayer())
    {
        if (const auto s_SpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
        {
            const auto s_mPlayerTransform = s_SpatialEntity->GetWorldMatrix();

            const auto s_Forward = (-s_mPlayerTransform.Backward).Normalized();
            const auto s_Right = s_mPlayerTransform.Right.Normalized();
            const auto s_Up = s_mPlayerTransform.Up.Normalized();

            SRepositoryPropInfo s_Item;
            if (m_sFixedRepositoryId.empty())
            {
                const auto& s_aRepositoryProps = GetRepositoryProps();
                s_Item = Math::SelectRandomElement(s_aRepositoryProps);
            }
            else
            {
				s_Item = GetRepositoryPropByID(m_sFixedRepositoryId);
            }

            for (size_t n = 0; n < m_nItemsPerBatch; n++)
            {
				auto s_vItemTrans = s_mPlayerTransform.Trans;
                s_vItemTrans +=
                    // spawn way above player
                    s_Up * c_fSpawnHeight

                    // spread out in radius
                    + s_Right * Math::GetRandomNumber(-c_fSpawnRadius, c_fSpawnRadius)
                    + s_Forward * Math::GetRandomNumber(-c_fSpawnRadius, c_fSpawnRadius);

                SpawnRepositoryPropAt(s_Item, s_vItemTrans);
            }
        }
    }
}

REGISTER_CHAOS_EFFECT_PARAM(random, ZItemRainEffect, "random", "Item Rain");
REGISTER_CHAOS_EFFECT_PARAM(cocaine, ZItemRainEffect,
    "cocaine",
    "White Snow",
    "c6e9414e-e2ce-470a-95bd-14cd25225878", // Melee Cocaine Brick
    0.2f,
    25
);
REGISTER_CHAOS_EFFECT_PARAM(explosive, ZItemRainEffect,
    "explosive",
    "Meteor Shower",
	"c82fefa7-febe-46c8-90ec-c945fbef0cb4", // Octane Booster
    0.2f,
    5
);