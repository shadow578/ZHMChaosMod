#include "IChaosEffect.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZResourceID.h>

#include "Helpers/ZTemplateEntitySpawner.h"
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"
#include "Entity/Bindings/SSoundFXPlayerEntityBinding.h"

/**
 * Reusable base for spawning sound effect players in the world.
 * Automatically handles resource loading, unloading and spawning.
 * Spawn sound effect players using the PlayAt() method.
 */
class ZSoundFXEffectBase : public virtual IChaosEffect, public virtual ZCompanionModDependentEffectBase
{
  public:
    ZSoundFXEffectBase() : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 4, 0)) // (new) SFXPlayer in 1.4.0
    {
    }

    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    bool Available() const override;

  protected:
    /**
     * Play a sound effect at the given position
     * @param p_mPosition position to play from
     * @param p_ridSound sound to play. Requires .pc_wwisebank resource that is already loaded.
     * @param p_bMusic whether the sound is music (uses ZMusicEventEntity) or sfx (uses ZAudioEventEntity)
     */
    SSoundFXPlayerEntityBinding PlayAt(const SMatrix& p_mPosition, const ZRuntimeResourceID& p_ridSound, const bool p_bMusic = false);
    SSoundFXPlayerEntityBinding CreatePlayer(const ZRuntimeResourceID& p_ridSound);

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pSoundPlayerSpawner;
    bool m_bDebugAsMusic = false;
};
