#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"

class ZFireworksEffect : public IChaosEffect
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    bool Available() const override;
    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVotingDisplay) const override
    {
        return "Fireworks!";
    }

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pFireworksSpawner;
    ZEntityRef m_rFireworksEntity;
};
