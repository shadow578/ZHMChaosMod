#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>

class ZLargeMinimapEffect : public IChaosEffect
{
  public:
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    void LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration) override;
    void DrawConfigUI(ZConfigurationAccessor* p_pConfiguration) override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Maximap";
    }    

  private: // config
    bool m_bReduceAlpha = false;

  private:
    ZEntityRef m_rMinimapContainer;
    ZEntityRef m_rMinimapHUDStatusMarkers;

    void SetMaximap(const bool p_bEnabled);
};
