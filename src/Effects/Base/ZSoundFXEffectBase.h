#include "IChaosEffect.h"

#include <Glacier/ZMath.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZResourceID.h>

#include "Helpers/ZTemplateEntitySpawner.h"

class ZSoundFXEffectBase : public virtual IChaosEffect
{
public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;

    bool Available() const override;

    ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
    {
        return IChaosEffect::AlwaysActiveLifecycleMethods() |
            ELifecycleMethodFlag::OnClearScene;
    }

protected:
    ZEntityRef PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource);

private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pSoundPlayerSpawner;
};
