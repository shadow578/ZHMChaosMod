#include "ZYoutubeVotingIntegration.h"

#include "EffectRegistry.h"
#include "Helpers/Utils.h"

ZYoutubeVotingIntegration::ZYoutubeVotingIntegration()
	: m_pYoutube(std::make_unique<ZYoutubeClient>("623315752739-3dikb4pf1lkgnd3n6e59l6qrprsk4q43.apps.googleusercontent.com", false)) // TODO developer ID
{
}

void ZYoutubeVotingIntegration::StartVote(const std::vector<IChaosEffect*>& p_aOptions)
{
	m_aActiveVote = p_aOptions;
}

IChaosEffect* ZYoutubeVotingIntegration::EndVote()
{
	return m_aActiveVote[0];
}

void ZYoutubeVotingIntegration::DrawConfigUI()
{
	if (ImGui::Button("start auth"))
	{
		m_pYoutube->Connect(false);
		Utils::CopyToClipboard(m_pYoutube->GetAuthorizationUrl());
	}
}

void ZYoutubeVotingIntegration::DrawOverlayUI()
{
	
}

REGISTER_VOTING_INTEGRATION(ZYoutubeVotingIntegration);
