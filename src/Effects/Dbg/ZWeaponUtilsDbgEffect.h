#pragma once
#include "Effects/Base/Companion/ZActorWeaponCustomControlEffectBase.h"

#include <Glacier/ZEntity.h>

class ZActor;

/**
 * Debug Effect for testing ActorUtil weapon-related helpers.
 */
class ZWeaponUtilsDbgEffect final : public ZActorWeaponCustomControlEffectBase
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