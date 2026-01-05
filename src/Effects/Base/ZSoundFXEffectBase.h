#include "IChaosEffect.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZResourceID.h>

#include "Helpers/ZTemplateEntitySpawner.h"

/**
 * Reusable base for spawning sound effect players in the world.
 * Automatically handles resource loading, unloading and spawning.
 * Spawn sound effect players using the PlayAt() method.
 */
class ZSoundFXEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    bool Available() const override;

protected:
    /**
     * Play a sound effect at the given position
     * @param p_Position position to play from
     * @param p_SoundResource sound effect to play. Requires .pc_wwisebank resource.
     */
    ZEntityRef PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource);

private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pSoundPlayerSpawner;
};
