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


	if (m_pYoutube->IsConnected())
	{
		ImGui::TextUnformatted(fmt::format("Connected to Stream {}",
			m_pYoutube->GetBroadcastConnection()->GetBroadcastInfo().m_sTitle
		).c_str());
	}

	if (ImGui::Button("install callbacks"))
	{
		m_pYoutube->GetBroadcastConnection()->SetOnChatMessageCallback(
			[this](const YT::SLiveChatMessage& p_Message)
			{
				Logger::Info("Chat message from {}: {}", p_Message.m_sAuthorName, p_Message.m_sMessageText);
			}
		);

		m_pYoutube->GetBroadcastConnection()->SetOnPollUpdateCallback(
			[this](const YT::SLivePollDetails& p_Poll)
			{
				Logger::Info("Poll update: {}", p_Poll.m_sQuestionText);
				for (const auto& s_Option : p_Poll.m_aOptions)
				{
					Logger::Info(" - {}: {} votes", s_Option.m_sOptionText, s_Option.m_nVoteCount);
				}
			}
		);
	}

	static YT::SLivePollDetails s_PollDetails;
	if (ImGui::Button("create poll"))
	{
		s_PollDetails.m_sQuestionText = "Hello from CPP!";
		s_PollDetails.m_aOptions = {
			{ "Hello!", 0 },
			{ "Hi There!", 0 },
			{ "Howdy!", 0 },
		};
		m_pYoutube->GetBroadcastConnection()->CreateLivePoll(s_PollDetails);
	}

	if (ImGui::Button("end poll"))
	{
		m_pYoutube->GetBroadcastConnection()->EndLivePoll(s_PollDetails);

		Logger::Info("Poll update: {}", s_PollDetails.m_sQuestionText);
		for (const auto& s_Option : s_PollDetails.m_aOptions)
		{
			Logger::Info(" - {}: {} votes", s_Option.m_sOptionText, s_Option.m_nVoteCount);
		}
	}

	if (ImGui::Button("send msg"))
	{
		YT::SLiveChatMessage s_Message{
			.m_sMessageText = "Hello from CPP!"
		};

		m_pYoutube->GetBroadcastConnection()->SendChatMessage(s_Message);
	}
}

void ZYoutubeVotingIntegration::DrawOverlayUI()
{
	
}

REGISTER_VOTING_INTEGRATION(ZYoutubeVotingIntegration);
