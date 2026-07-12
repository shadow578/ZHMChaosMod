#pragma once
#include "Effects/Base/Companion/ZActorFollowPlayerHelperEffectBase.h"

class ZActorsFollowPlayerEffect : public ZActorFollowPlayerHelperEffectBase
{
  public:
    ZActorsFollowPlayerEffect(const bool p_bIgnoreAllElse)
        : ZActorFollowPlayerHelperEffectBase(),
          m_bActorsIgnoreAllElse(p_bIgnoreAllElse)
    {
    }

    void Start() override;
    void Stop() override;

    std::string GetName() const override
    {
        const auto s_sSuffix = m_bActorsIgnoreAllElse ? "_nobrain" : "_normal";
        return IChaosEffect::GetName() + s_sSuffix;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_bActorsIgnoreAllElse ? "The Walking Dead" : "You Are Famous";
    }

    EDuration GetDuration() const override
    {
        return m_bActorsIgnoreAllElse ? EDuration::Short : EDuration::Full;
    }

  private:
    const bool m_bActorsIgnoreAllElse;

    void SetActorsFollowPlayer(const bool p_bFollow);
};
