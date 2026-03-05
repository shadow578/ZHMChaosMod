#pragma once
#include "IVotingIntegration.h"

class ZOfflineVoting : public IVotingIntegration
{
  public:
    void StartVote(const std::vector<std::shared_ptr<IChaosEffect>>& p_aOptions) override;
    std::shared_ptr<IChaosEffect> EndVote() override;

    void DrawOverlayUI() override;

    std::string GetDisplayName() const override
    {
        return "Offline";
    }

  private:
    std::vector<std::shared_ptr<IChaosEffect>> m_aActiveVote;
};
