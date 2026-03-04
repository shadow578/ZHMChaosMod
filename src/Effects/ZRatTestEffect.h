#pragma once
#include "IChaosEffect.h"
#include "Helpers/ZTemplateEntitySpawner.h"

class ZRatTestEffect : public IChaosEffect
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    bool Available() const override;

    void Start();

    void OnDrawDebugUI() override;

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pSpawner;
};
