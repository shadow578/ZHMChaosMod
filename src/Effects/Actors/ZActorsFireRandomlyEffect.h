#pragma once
#include "Effects/Base/Companion/ZActorWeaponCustomControlEffectBase.h"

class ZActorsFireRandomlyEffect : public ZActorWeaponCustomControlEffectBase
{
  public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "This Is America";
    }

    void LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration) override;
    void DrawConfigUI(ZConfigurationAccessor* p_pConfiguration) override;

  private:
    bool m_bArmAllActors = false;

    void SetAllActorsShootingContinuous(const bool p_bShooting);
};
