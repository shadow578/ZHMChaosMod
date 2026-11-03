#pragma once
#include "Effects/Base/Companion/ZActorUtilsEffectBase.h"

#include <Glacier/ZEntity.h>

class ZActor;

/**
 * Debug Effect for testing Actor Utils functionality.
 */
class ZActorUtilsDbgEffect final : public ZActorUtilsEffectBase
{
  public:
    void Start() override {}

    void OnDrawDebugUI() override;

    bool IsEnabled() const override
    {
        return false;
    }

  private:
    TEntityRef<ZActor> m_rTargetActor;

  private: // ActorAIPerceptibilityDisabled
    void DrawUIForActorAIPerceptibilityDisabled();

  private: // ActorProxy
    void DrawUIForActorProxy();

  private: // AIGoal
    void DrawUIForAIGoal();

  private: // AIModifier
    void DrawUIForAIModifier();

  private: // RagdollAttacher
    void DrawUIForRagdollAttacher();

  private: // SpeakEntity
    void DrawUIForSpeakEntity();
};