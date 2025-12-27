#pragma once
#include "Effects/Base/ZPoisonAOEDamageEffectBase.h"
#include "Effects/Base/ZSoundFXEffectBase.h"

class ZPlayerFlatulenceEffect : public ZPoisonAOEDamageEffectBase, public ZSoundFXEffectBase
{
    void LoadResources() override;
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
};