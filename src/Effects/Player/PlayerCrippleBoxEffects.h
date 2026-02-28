#pragma once
#include "Effects/Base/ZPlayerCrippleBoxEffectBase.h"

class ZUnlimitedAmmoEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;
};

class ZPlayerPacifistEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;
};

class ZPlayerForceCrouchEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect *p_pOther) const override;
};

class ZPlayerSlowMovementEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;

    bool IsCompatibleWith(const IChaosEffect *p_pOther) const override;
};

class ZPlayerNoInventoryEffect : public ZPlayerCrippleBoxEffectBase
{
  public:
    void Start() override;
};
