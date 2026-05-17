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
    char m_szActorName[256] = "John Doe";
    char m_szOutfitRepositoryID[256] = "3acdec27-0f5d-4e66-aadd-2e9a9751b2a0";
    char m_szOutfitCommonName[256] = "Crowd_Civilian_European_Casual_M";
    int m_nCharsetIndex = 0;
    int m_nOutfitVariationIndex = 0;
    TEntityRef<ZActor> m_rTargetActor;
};