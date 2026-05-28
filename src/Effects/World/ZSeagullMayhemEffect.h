#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

class ZSeagullMayhemEffect : public virtual IChaosEffect, public virtual ZCompanionModDependentEffectBase
{
  public:
    ZSeagullMayhemEffect() : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 4, 0)) // adds CM_SeagullMayhem_PORT entity template in 1.4.0
    {
    }

    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    bool Available() const override;

    void Start() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Seagull Mayhem";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Full;
    }

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pEffectSpawner;
    bool m_bDoSpawnOnNextUpdate = false;

    bool m_bRandomizeParticleCount = true;
    float32 m_fParticleCount = 150.f;
    float32 m_fEmissionTime = 5.f;
};