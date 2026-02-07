#pragma once
#include "IVotingIntegration.h"

class ZYoutubeAuthHandler;
class ZAuthToken;
class ZYoutubeBroadcastConnection;

class ZYoutubeIntegrationBase : public IVotingIntegration
{
public:
	ZYoutubeIntegrationBase(const bool p_bReadOnly);

	void Deactivate() override;

	void StartVote(const std::vector<IChaosEffect*>& p_aOptions) override;
	IChaosEffect* EndVote() override;
	void DrawConfigUI() override;

protected:
	std::unique_ptr<ZYoutubeAuthHandler> m_pYoutubeAuth;
	std::shared_ptr<ZYoutubeBroadcastConnection> m_pCurrentBroadcast;

	std::vector<IChaosEffect*> m_aActiveVote;

	virtual void OnBroadcastConnected() = 0;

	virtual void StartVoteImpl() = 0;
	virtual IChaosEffect* EndVoteImpl() = 0;

private:
	void ConnectCurrentBroadcast();
	void Disconnect();
};
