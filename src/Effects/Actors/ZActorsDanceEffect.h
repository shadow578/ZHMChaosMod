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
    enum class EDanceStyle
    {
        Lambic,   // Act_MR_Lambic_Dance
        DanceMat, // Act_MR_Stand_Dance_Mat
        Flamingo  // Act_MR_Stand_Mascot_Entertain
    };

    EDanceStyle m_DanceStyle = EDanceStyle::Lambic;

    void SetAllActorsDancing(const bool p_bDancing);
};
