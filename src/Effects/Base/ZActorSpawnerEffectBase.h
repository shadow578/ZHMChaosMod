#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZMath.h>

#include "Helpers/ZTemplateEntitySpawner.h"

/**
 * Reuseable base for spawning new actors.
 * Use SpawnActor() method to spawn new actors. Lifecycle is handled automatically for you.
 */
class ZActorSpawnerEffectBase : public virtual IChaosEffect
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

  protected:
    /**
     * Spawn a new actor at the given position, equipping the given outfit.
     * @param p_mPosition Position & orientation of the spawned actor.
     * @param p_sName Name of the actor.
     * @param p_sOutfitRepositoryId Repository ID of the outfit to equip the actor with. 
     *        Requires a outfit of type Actor. 
     *        Note that the global outfit kit must be available. This method will not load it!
     * @param p_nCharsetIndex Index of the charset to use. Only used if the outfit has multiple charsets.
     * @param p_nOutfitVariationIndex Index of the outfit variation to use.
     * @note The spawned actor will inherit some behavior from the outfit template, but will mostly just stand and do nothing.
     *       Utilize e.g. the act library helper to make the actor do something interesting.
     */
    TEntityRef<ZActor> SpawnActor(const SMatrix m_mPosition, const std::string& p_sName, const std::string& p_sOutfitRepositoryId, const uint8_t p_nCharsetIndex = 0, const uint8_t p_nOutfitVariationIndex = 0);

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pActorSpawner;
};
