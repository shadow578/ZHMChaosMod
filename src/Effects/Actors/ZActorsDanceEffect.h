#pragma once
#include "Effects/Base/Companion/ZActorActLibraryEffectBase.h"

class ZActorsDanceEffect : public ZActorActLibraryEffectBase
{
  public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Everybody Dance Now!";
    }

  private:
    bool m_bUseLambicDance = false;

    void SetAllActorsDancing(const bool p_bDancing);
};
