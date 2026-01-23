#pragma once
#include "ZYoutubeIntegrationBase.h"

class ZChatVoting;

class ZYoutubeChatVotingIntegration : public ZYoutubeIntegrationBase
{
public:
	ZYoutubeChatVotingIntegration();

	void DrawConfigUI() override;
	void DrawOverlayUI() override;

	std::string GetDisplayName() const override
	{
		return "YouTube Chat Voting";
	}

protected:
	void OnBroadcastConnected() override;

	void StartVoteImpl() override;
	IChaosEffect* EndVoteImpl() override;

private:
	std::unique_ptr<ZChatVoting> m_pVoting;

	bool m_bMembersOnlyVoting = false;
};