#pragma once
#include "Effects/Base/ZActorSpawnerEffectBase.h"
#include "Effects/Base/Companion/ZActorActLibraryEffectBase.h"

class ZActorDanceGroupEffect : public ZActorSpawnerEffectBase, public ZActorActLibraryEffectBase
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

    void Start() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

    void LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration) override;
    void DrawConfigUI(ZConfigurationAccessor* p_pConfiguration) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Dance Party";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

  private: // config
    int m_nCount = 6;
    float32 m_fRadius = 3.f;
    float32 m_fTimeDelayToActActivation = 0.5f;

  private:
    float32 m_fRemainingTimeToActActivation = 0.f;
    std::vector<TEntityRef<ZActor>> m_aDancersPendingActivation;

    void SpawnAt(const SMatrix& p_mCenter);
    std::vector<SMatrix> GetSpawnPositions(const float4& p_vCenter) const;
};