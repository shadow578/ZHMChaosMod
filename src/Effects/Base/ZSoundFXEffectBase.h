#include "IChaosEffect.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZResourceID.h>

class ZSoundFXEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    bool Available() override;
    void OnDrawDebugUI() override;

protected:
    void PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource);

private:
    bool SpawnEntity(ZEntityRef& p_RootEntity);
};
