#pragma once
#include "Effects/Base/ZPlayerCrippleBoxEffectBase.h"

class ZUnlimitedAmmoEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Unlimited Ammo";
    }
};

class ZPlayerPacifistEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Pacifist Mode";
    }
};

class ZPlayerForceCrouchEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Goblin Mode";
    }
};

class ZPlayerSlowMovementEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Captain Slow";
    }
};

class ZPlayerNoInventoryEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "No Pockets";
    }
};
