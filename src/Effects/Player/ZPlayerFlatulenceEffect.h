#pragma once
#include "Effects/Base/ZPoisonAOEDamageEffectBase.h"
#include "Effects/Base/ZSoundFXEffectBase.h"

class ZPlayerFlatulenceEffect : public ZPoisonAOEDamageEffectBase, public ZSoundFXEffectBase
{
    void LoadResources() override;
    void OnDrawDebugUI() override;
    bool Available() override;
    void Start() override;

    std::string GetDisplayName() override
    {
        return "Flatulence";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }
};