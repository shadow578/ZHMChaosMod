#pragma once
#include "Effects/Base/ZPlayerAICrippleEffectBase.h"

class ZPlayerInvincibleEffect : public ZPlayerAICrippleEffectBase
{
  public:
    void Start() override;
    void Stop() override;
};

class ZPlayerHiddenEffect : public ZPlayerAICrippleEffectBase
{
  public:
    void Start() override;
    void Stop() override;
};
