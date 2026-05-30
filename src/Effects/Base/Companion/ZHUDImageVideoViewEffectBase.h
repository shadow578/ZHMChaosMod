#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Entity/Bindings/SImageVideoViewEntityBinding.h"

/**
 * Base class for accessing HUD Image / Video view added by companion mod.
 * Note that you'll have to handle loading of the image / video resource yourself (via ZResourceProvider).
 */
class ZHUDImageVideoViewEffectBase : public virtual ZCompanionModDependentEffectBase
{
  public:
    ZHUDImageVideoViewEffectBase()
        : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 4, 0)) // Image / Video view added in 1.4.0
    {
    }

    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    void OnDrawDebugUI() override;

  protected:
    SImageVideoViewEntityBinding GetImageAndVideoViewBinding() const;

  private:
    ZEntityRef m_rImageVideoView;
};
