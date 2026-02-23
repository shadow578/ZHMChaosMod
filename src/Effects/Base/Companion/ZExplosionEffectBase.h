#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"

#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

/**
 * Reusable base for spawning explosions in the game world.
 * Handles Resource loading and cleanup, as well as spawning.
 * Spawn explosions using the SpawnExplosion() method.
 */
class ZExplosionEffectBase : public virtual IChaosEffect, public virtual ZCompanionModDependentEffectBase
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

protected:
    struct SExplosionParams
    {
        /**
         * Position the explosion is spawned at (center).
         */
        SMatrix m_Position;

        /**
         * Minimum delay to detonation, seconds.
         */
        float32 m_fFuseTimeMin = 0.1f;

        /**
         * Maximum delay to detonation, seconds.
         */
        float32 m_fFuseTimeMax = 0.3f;

        /**
         * Explosion strength
         */
        float32 m_fTargetStrength = 100.0f;

        /**
         * How deaths caused by the explosion should be handled in-game.
         */
        EDeathContext m_eDeathContext = EDeathContext::eDC_MURDER;

        /**
         * What kind of death the explosion causes.
         */
        EDeathType m_eDeathType = EDeathType::eDT_KILL;
    };

    ZEntityRef SpawnExplosion(const SExplosionParams& p_Params);

private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pExplosionSpawner;
};
