#pragma once
#include "Effects/Base/Companion/ZSoundFXEffectBase.h"
#include "Effects/Base/Companion/ZActorActLibraryEffectBase.h"

#include "Helpers/ZResourceProvider.h"

class ZActorsDanceEffect : public ZActorActLibraryEffectBase, public ZSoundFXEffectBase
{
  public:
    void LoadResources() override;
    void OnClearScene() override;
    void OnDrawDebugUI() override;
    bool Available() const override;

    void Start() override;
    void Stop() override;

  private:
    std::unique_ptr<ZResourceProvider> m_pSFXResource;
    std::vector<SDanceMatActBinding> m_aActiveActBindings;

    std::array<SMatrix, 6> GetDancePositions(const float4& p_vBase) const;
};
