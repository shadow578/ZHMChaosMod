#pragma once
#include "IVotingIntegration.h"

#include "TwitchClient.h"

class ZTwitchVotingIntegration : public IVotingIntegration
{
  public:
    ZTwitchVotingIntegration();

    void StartVote(const std::vector<std::shared_ptr<IChaosEffect>>& p_aOptions) override;
    std::shared_ptr<IChaosEffect> EndVote() override;

    void DrawConfigUI() override;
    void DrawOverlayUI() override;

    std::string GetDisplayName() const override
    {
        return "Twitch";
    }

  private:
    std::unique_ptr<TwitchClient> m_pTwitch;
    std::vector<std::shared_ptr<IChaosEffect>> m_aActiveVote;

    std::shared_ptr<IChaosEffect> EndVoteTwitch();
};
