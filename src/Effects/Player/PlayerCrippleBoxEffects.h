#pragma once
#include "Effects/Base/ZPlayerCrippleBoxEffectBase.h"

class ZUnlimitedAmmoEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const
    {
        return "Unlimited Ammo";
    }
};

class ZPlayerPacifistEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const
    {
        return "Pacifist Mode";
    }
};

class ZPlayerForceCrouchEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    std::string GetDisplayName(const bool p_bVoting) const
    {
        return "Goblin Mode";
    }
};

class ZPlayerSlowMovementEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    std::string GetDisplayName(const bool p_bVoting) const
    {
        return "Arthritis Mode";
    }
};

class ZPlayerNoInventoryEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const
    {
        return "No Pockets";
    }
};
