#pragma once
#include "Effects/Base/ZActorSpawnerEffectBase.h"

/**
 * Debug Effect for testing Actor Spawner capabilities.
 */
class ZActorSpawnerDbgEffect final : public ZActorSpawnerEffectBase
{
  public:
    void Start() override {}

    void OnDrawDebugUI() override;

    bool IsEnabled() const override
    {
        return false;
    }

  private:
    char m_szOutfitCommonName[256];
    TEntityRef<ZActor> m_rTargetActor;
};