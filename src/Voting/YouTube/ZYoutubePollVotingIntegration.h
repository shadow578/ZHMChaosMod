#pragma once
#include "ZYoutubeIntegrationBase.h"

#include "Client/Model.h"

#include <atomic>
#include <mutex>

class ZBackgroundWorker;

class ZYoutubePollVotingIntegration : public ZYoutubeIntegrationBase
{
  public:
    ZYoutubePollVotingIntegration();

    void DrawConfigUI() override;
    void DrawOverlayUI() override;

    std::string GetDisplayName() const override
    {
        return "YouTube Live Poll";
    }

  protected:
    void OnBroadcastConnected() override;

    void StartVoteImpl() override;
    std::shared_ptr<IChaosEffect> EndVoteImpl() override;

  private:
    std::unique_ptr<ZBackgroundWorker> m_pWorker;

    std::atomic<bool> m_bVoteActive = false;
    bool m_bShowVoteCountsInOverlay = true;

    std::recursive_mutex m_PollDetailsMutex;
    YT::SLivePollDetails m_CurrentPollDetails;
};