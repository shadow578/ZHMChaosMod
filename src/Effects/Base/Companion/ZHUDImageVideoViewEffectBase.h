#pragma once
#include "Effects/Base/Companion/ZCompanionModDependentEffectBase.h"

#include "Helpers/ZTemplateEntitySpawner.h"

#include "Entity/Bindings/SImageVideoViewEntityBinding.h"

/**
 * Base class for accessing HUD Image / Video view added by companion mod.
 * Note that you'll have to handle loading of the image / video resource yourself (via ZResourceProvider).
 * Use of non-loaded resources (or ones not contained in chunk0) may crash the game.
 * Also note that, at a maximum 6 videos can be played via this (across ALL effects).
 * For images, there seems to be no such limit.
 */
class ZHUDImageVideoViewEffectBase : public virtual ZCompanionModDependentEffectBase
{
  public:
    ZHUDImageVideoViewEffectBase()
        : ZCompanionModDependentEffectBase(CompanionModUtil::SVersion(1, 4, 0)) // Image / Video view added in 1.4.0
    {
    }

    void LoadResources() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

  protected:
    SImageVideoViewEntityBinding SpawnImageAndVideoView();

  private:
    std::unique_ptr<ZTemplateEntitySpawner> m_pViewSpawner;

    ZEntityRef m_rLastSpawnedViewEntity; // for debug UI
};
