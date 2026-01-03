#pragma once

#include "IChaosEffect.h"

#include <Glacier/SColorRGB.h>

#include "Helpers/ZTemplateEntitySpawner.h"

class ZPoisonAOEDamageEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

	bool Available() const override;

    ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
    {
        return IChaosEffect::AlwaysActiveLifecycleMethods() |
            ELifecycleMethodFlag::OnClearScene;
    }

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
        SMatrix m_Position;
        EPoisonType m_eType;
        SVector3 m_AreaSize = SVector3(4.0f, 4.0f, 4.0f);
        SColorRGB m_ParticleColorRangeStart{ .r = 255, .g = 0, .b = 0 };
        SColorRGB m_ParticleColorRangeEnd{ .r = 255, .g = 0, .b = 0 };
    };

    ZEntityRef Spawn(const SParams& p_Params);

private:
    bool GetPoisonKeywordEntity(const EPoisonType p_eType, ZEntityRef p_RootEntity, ZEntityRef& p_KeywordEntity);

    std::unique_ptr<ZTemplateEntitySpawner> m_pEffectCloudSpawner;
    EPoisonType m_eDebugPoisonType = EPoisonType::SICK;
};
