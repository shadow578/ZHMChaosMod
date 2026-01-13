#pragma once
#include "IVotingIntegration.h"

#include "TwitchClient.h"

class ZTwitchVotingIntegration : public IVotingIntegration
{
public:
	ZTwitchVotingIntegration();

	void StartVote(const std::vector<IChaosEffect*>& p_aOptions) override;
	IChaosEffect* EndVote() override;

	void DrawConfigUI() override;
	void DrawOverlayUI() override;

	std::string GetDisplayName() const override
	{
		return "Twitch";
	}

private:
	std::unique_ptr<TwitchClient> m_pTwitch;
	std::vector<IChaosEffect*> m_aActiveVote;

	IChaosEffect* EndVoteTwitch();
};
