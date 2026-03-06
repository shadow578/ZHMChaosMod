#pragma once
#include "Effects/Base/ZActorWellbeingChangeEffectBase.h"
#include "Effects/Base/ZSpawnRepositoryItemEffectBase.h"

class ZActorToWineEffect : public ZActorWellbeingChangeEffectBase, public ZSpawnRepositoryItemEffectBase
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
        return "VINO HUMANO";
    }

  protected:
    void OnActorWellbeingChanged(ZActor* p_pActor, const SActorState& p_OldState, const SActorState& p_NewState) override;

  private:
    SRepositoryPropInfo m_WineBottleProp;
    bool m_bTeleportBodies = true;

    void OnActorPerished(ZActor* p_pActor);
};
