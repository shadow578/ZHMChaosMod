#pragma once
#include "Effects/Base/Companion/ZHUDImageVideoViewEffectBase.h"

#include "Helpers/ZResourceProvider.h"

#include "Entity/Bindings/SZUIControlEntityBinding.h"

#include <vector>

// IOI calls their crosshair a "reticle" bc they're fancy.
class ZCustomReticleEffect : public ZHUDImageVideoViewEffectBase
{
  public:
    ZCustomReticleEffect(
        const std::string& p_sNameSuffix,
        const std::string& p_sDisplayNameSuffix,
        const std::string& p_sImageResourcePath,
        const bool p_bAllowFlipHorizontal = false,
        const bool p_bAllowFlipVertical = false
    )
        : ZHUDImageVideoViewEffectBase(),
          m_sNameSuffix(p_sNameSuffix),
          m_sDisplayNameSuffix(p_sDisplayNameSuffix),
          m_sImageResourcePath(p_sImageResourcePath),
          m_bAllowFlipHorizontal(p_bAllowFlipHorizontal),
          m_bAllowFlipVertical(p_bAllowFlipVertical)
    {
    }

    void LoadResources() override;
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void OnDrawDebugUI() override;

    void Start();
    void Stop();
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

    std::string GetName() const override
    {
        return ZHUDImageVideoViewEffectBase::GetName() + "_" + m_sNameSuffix;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        std::string s_sName = "Aim Assist";
        if (!p_bVoting)
        {
            s_sName += " (" + m_sDisplayNameSuffix + ")";
        }

        return s_sName;
    }

  private:
    std::string m_sNameSuffix;
    std::string m_sDisplayNameSuffix;
    std::string m_sImageResourcePath;
    bool m_bAllowFlipHorizontal;
    bool m_bAllowFlipVertical;

  private:
    std::unique_ptr<ZResourceProvider> m_pImageResource;
    SImageVideoViewEntityBinding m_ReticleView;
    SZUIControlEntityBinding m_OriginalReticle;
    bool m_bTrackReticle = false;
};
