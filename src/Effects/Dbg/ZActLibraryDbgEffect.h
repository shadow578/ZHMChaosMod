#pragma once
#include "Effects/Base/Companion/ZActorActLibraryEffectBase.h"

#include <Glacier/ZEntity.h>

class ZActor;

/**
 * Debug Effect for testing Actor Act Library functionality.
 */
class ZActLibraryDbgEffect final : public ZActorActLibraryEffectBase
{
  public:
    void Start() override {}

    void OnDrawDebugUI() override;

    bool IsEnabled() const override
    {
        return false;
    }

  private:
    TEntityRef<ZActor> m_rTargetActor;

    void DrawUIForStandWaiting();
    void DrawUIForStandDanceMat();
    void DrawUIForLambicDance();
    void DrawUIForFlamingoDance();
};