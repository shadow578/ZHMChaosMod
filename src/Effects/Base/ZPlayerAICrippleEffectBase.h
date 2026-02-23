#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Helpers/EntityBindings/SAICrippleEntityBinding.h"

/**
 * Base effect for applying a ZAICrippleEntity to the player.
 * Use GetAICripple() to access the spawned AI Cripple and modify its properties as needed.
 * Spawning / Despawning and resource management is handled automatically.
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
