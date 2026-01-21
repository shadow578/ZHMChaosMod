#pragma once
#include "IVotingIntegration.h"

class ZYoutubeAuthHandler;
class ZYoutubeBroadcastConnection;
class ZChatVoting;

class ZYoutubeVotingIntegration : public IVotingIntegration
{
public:
	ZYoutubeVotingIntegration();

	void StartVote(const std::vector<IChaosEffect*>& p_aOptions) override;
	IChaosEffect* EndVote() override;

	void DrawConfigUI() override;
	void DrawOverlayUI() override;

private:
	std::vector<IChaosEffect*> m_aActiveVote;

	std::unique_ptr<ZYoutubeAuthHandler> m_pAuth;
	std::shared_ptr<ZYoutubeBroadcastConnection> m_pBroadcast;

	std::unique_ptr<ZChatVoting> m_pChatVote;
};
