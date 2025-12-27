#pragma once
#include "Effects/Base/ZActorWellbeingChangeEffectBase.h"
#include "Effects/Base/ZExplosionEffectBase.h"

class ZExplosiveKnockoutEffect : public ZActorWellbeingChangeEffectBase, public ZExplosionEffectBase
{
public:
    std::string GetDisplayName() const override
    {
        return "Explosive Knockout";
    }

protected:
    void OnActorWellbeingChanged(ZActor* p_pActor, const SActorState& p_OldState, const SActorState& p_NewState) override;

private:
    void SpawnExplosionAtActor(ZActor* p_pActor, const float32 p_fStrength);
};
