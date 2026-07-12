#pragma once
#include "Effects/Base/Companion/ZActorFollowPlayerHelperEffectBase.h"

#include <Glacier/ZEntity.h>

class ZActor;

/**
 * Debug Effect for testing actor follow player helper.
 */
class ZActorFollowPlayerDbgEffect final : public ZActorFollowPlayerHelperEffectBase
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
};