#pragma once
#include "Effects/Base/ZPlayerAICrippleEffectBase.h"

class ZPlayerInvincibleEffect : public ZPlayerAICrippleEffectBase
{
  public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "God Mode";
    }
};

class ZPlayerHiddenEffect : public ZPlayerAICrippleEffectBase
{
  public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Now You See Me";
    }
};
