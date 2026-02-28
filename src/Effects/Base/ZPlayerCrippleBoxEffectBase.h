#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"

#include <Glacier/ZHM5CrippleBox.h>

/**
 * Base effect for applying a ZHM5CrippleBox to the player.
 * Use GetCrippleBox() to access the spawned CrippleBox and modify its properties as needed.
 * Assignment of the player entity and deactivation is handled automatically.
 * The effect must however call ActivateCrippleBox() to apply any properties set on the cripple box.
 * Spawning / Despawning and resource management is handled automatically.
 * @note for documentation on what the flags do, see tools/ZHM5CrippleBox_flags.md
 * @note this base effect does NOT define `IsCompatibleWith` to block activation with other effects using a cripple box.
 * Multiple cripple boxes can be active at the same time. As long as they don't modify the same flags, they work fine together.
 * Effects must ensure that they implement `IsCompatibleWith` to block any effects modifying the same flags.
 */
class ZPlayerCrippleBoxEffectBase : public virtual IChaosEffect
{
  public:
    void LoadResources() override;
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

    void Stop() override;

  protected:
    /**
     * Get the spawned cripple box entity reference.
     * note that after configuring the cripple box, you must call ActivateCrippleBox() to apply the changes.
     */
    inline TEntityRef<ZHM5CrippleBox> GetCrippleBox() const
    {
        return m_rCrippleBox;
    }

    /**
     * Activate the cripple box with the currently set configuration.
     */
    void ActivateCrippleBox();

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pCrippleBoxSpawner;
    TEntityRef<ZHM5CrippleBox> m_rCrippleBox;
};

/**
 * Helper effect for testing Cripple Box flags.
 * NOT INTENDED FOR USE IN ACTUAL EFFECTS.
 */
class ZPlayerCrippleBoxDbgEffect final : public virtual ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override {}

    void OnDrawDebugUI() override;

  private:
    bool m_bReapplyOnChange = true;
};
