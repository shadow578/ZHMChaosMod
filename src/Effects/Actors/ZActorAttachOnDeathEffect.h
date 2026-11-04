#pragma once
#include "Effects/Base/ZActorWellbeingChangeEffectBase.h"
#include "Effects/Base/Companion/ZActorUtilsEffectBase.h"

#include <vector>

class ZActorAttachOnDeathEffect : public ZActorWellbeingChangeEffectBase, public ZActorUtilsEffectBase
{
  public:
    bool Available() const override;

    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

  protected:
    void OnActorWellbeingChanged(TEntityRef<ZActor> p_rActor, const SActorState& p_OldState, const SActorState& p_NewState) override;

  private:
    std::vector<SZRagdollAttacherEntityBinding> m_aRagdollBindings;
    float m_fUpwardsOffset = 1.2f;
    float m_fDetachDistance = 1.5f;
    bool m_bDetachOnContact = false;
    bool m_bParentToPlayerOnContact = false;
    bool m_bActive = false;

    void OnActorPerished(TEntityRef<ZActor> p_rActor);
};
