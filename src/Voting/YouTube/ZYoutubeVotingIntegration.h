#pragma once
#include "IVotingIntegration.h"

#include "ZYoutubeClient.h"

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

	std::unique_ptr<ZYoutubeClient> m_pYoutube;
};
