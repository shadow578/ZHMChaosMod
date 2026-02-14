#pragma once

#include "IChaosEffect.h"

#include <Glacier/SColorRGB.h>

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Effects/Base/ZCompanionModDependentEffectBase.h"

/**
 * Reusable base for spawning Poison Area-of-Effect Damage Clouds.
 * Handles resource loading, unloading and spawning.
 * Spawn a AOE Cloud using Spawn().
 */
class ZPoisonAOEDamageEffectBase : public virtual IChaosEffect, public virtual ZCompanionModDependentEffectBase
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

	bool Available() const override;

    // Note: poison type values must match those of the Keyword_ITEM_POISON_* entities
    // in the g_AOECloudProp template.
    // for debug to work, you must also add any new one in OnDrawDebugUI().
    enum class EPoisonType
    {
        SICK = 0xe001,
        SEDATIVE = 0xe002,
        LETHAL_SLOW = 0xe003,
        LETHAL_FAST = 0xe004
    };

protected:
    struct SParams
    {
        /**
         * Center position of the AOE box.
         */
        SMatrix m_Position;

        /**
         * Type of poison damage to apply.
         */
        EPoisonType m_eType;

        /**
         * Size of the AOE box.
         */
        SVector3 m_AreaSize = SVector3(4.0f, 4.0f, 4.0f);

        /**
         * Colors of the Particle effect.
         */
        SColorRGB m_ParticleColorRangeStart{ .r = 255, .g = 0, .b = 0 };
        SColorRGB m_ParticleColorRangeEnd{ .r = 255, .g = 0, .b = 0 };
    };

    ZEntityRef Spawn(const SParams& p_Params);

private:
    bool GetPoisonKeywordEntity(const EPoisonType p_eType, ZEntityRef p_RootEntity, ZEntityRef& p_KeywordEntity);

    std::unique_ptr<ZTemplateEntitySpawner> m_pEffectCloudSpawner;
    EPoisonType m_eDebugPoisonType = EPoisonType::SICK;
};
