#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"

class ZExplosionEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

protected:
    struct SExplosionParams
    {
        SMatrix m_Position;
        float32 m_fFuseTimeMin = 0.1f;
        float32 m_fFuseTimeMax = 0.3f;
        float32 m_fTargetStrength = 100.0f;
        EDeathContext m_eDeathContext = EDeathContext::eDC_MURDER;
        EDeathType m_eDeathType = EDeathType::eDT_KILL;
    };

    ZEntityRef SpawnExplosion(const SExplosionParams& p_Params);

private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pExplosionSpawner;
};
