#include "Effects/Base/Companion/ZHUDImageVideoViewEffectBase.h"

#include "Helpers/ZResourceProvider.h"

#include "Entity/Bindings/SZUIControlEntityBinding.h"

#include <vector>

// IOI calls their crosshair a "reticle" bc they're fancy.
class ZCustomReticleEffect : public ZHUDImageVideoViewEffectBase
{
  public:
    void LoadResources() override;
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

    void Start();
    void Stop();
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Aim Assist";
    }

  private:
    struct SReticleImageItem
    {
        std::shared_ptr<ZResourceProvider> m_pResource;
        bool m_bAllowFlipHorizontal;
        bool m_bAllowFlipVertical;
    };

    std::vector<SReticleImageItem> m_aReticleImages;
    SImageVideoViewEntityBinding m_ReticleView;
    SZUIControlEntityBinding m_OriginalReticle;
    bool m_bTrackReticle = false;
};
