#pragma once
#include "IVotingIntegration.h"

class ZOfflineVoting : public IVotingIntegration
{
public:
	void StartVote(const std::vector<IChaosEffect*>& p_aOptions) override;
	IChaosEffect* EndVote() override;

	void DrawOverlayUI() override;

	std::string GetDisplayName() const override
	{
		return "Offline";
	}

private:
	std::vector<IChaosEffect*> m_aActiveVote;
};
