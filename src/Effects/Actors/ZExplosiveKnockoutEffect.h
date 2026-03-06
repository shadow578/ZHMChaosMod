#pragma once
#include "Effects/Base/ZActorWellbeingChangeEffectBase.h"
#include "Effects/Base/Companion/ZExplosionEffectBase.h"

class ZExplosiveKnockoutEffect : public ZActorWellbeingChangeEffectBase, public ZExplosionEffectBase
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

    void Start() override;
    void Stop() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Explosive Knockout";
    }

  protected:
    void OnActorWellbeingChanged(ZActor* p_pActor, const SActorState& p_OldState, const SActorState& p_NewState) override;

  private:
    void SpawnExplosionAtActor(ZActor* p_pActor, const float32 p_fStrength);
};
