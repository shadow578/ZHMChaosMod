#pragma once
#include "Effects/Base/ZPoisonAOEDamageEffectBase.h"
#include "Effects/Base/ZSoundFXEffectBase.h"

#include "Helpers/ZResourceProvider.h"

class ZPlayerFlatulenceEffect : public ZPoisonAOEDamageEffectBase, public ZSoundFXEffectBase
{
public:
    void LoadResources() override;
	void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const  override;
    void Start() override;

    std::string GetDisplayName() const override
    {
        return "Flatulence";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

    ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
    {
        return ZPoisonAOEDamageEffectBase::AlwaysActiveLifecycleMethods() |
            ZSoundFXEffectBase::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnClearScene;
    }

private:
    std::unique_ptr<ZResourceProvider> m_pSFXResource;
};