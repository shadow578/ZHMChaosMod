#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Entity/Bindings/SAICrippleEntityBinding.h"

/**
 * Base effect for applying a ZAICrippleEntity to the player.
 * Use GetAICripple() to access the spawned AI Cripple and modify its properties as needed.
 * Spawning / Despawning and resource management is handled automatically.
 * @note this base effect does NOT implement `IsCompatibleWith` to block activation with other effects using an AI Cripple.
 * Multiple AI Cripple Entities can be active at the same time. As long as they don't affect the same AI behaviors, they work fine together.
 * Effects must ensure that they implement `IsCompatibleWith` to block any effects modifying the same AI behaviors.
 */
class ZPlayerAICrippleEffectBase : public virtual IChaosEffect
{
  public:
    void LoadResources() override;
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

  protected:
    inline SAICrippleEntityBinding GetAICripple() const
    {
        return SAICrippleEntityBinding(m_rAICripple);
    }

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pAICrippleSpawner;
    ZEntityRef m_rAICripple;
};
