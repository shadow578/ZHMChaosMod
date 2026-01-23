#include "ZYoutubeVotingIntegration.h"

#include "Client/ZYoutubeAuthHandler.h"
#include "Client/ZYoutubeBroadcastConnection.h"
#include "Client/ZAuthToken.h"

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/Voting/ZChatVoting.h"

ZYoutubeVotingIntegration::ZYoutubeVotingIntegration()
	: m_pAuth(std::make_unique<ZYoutubeAuthHandler>("623315752739-3dikb4pf1lkgnd3n6e59l6qrprsk4q43.apps.googleusercontent.com", false)), // TODO developer ID
	m_pBroadcast(nullptr),
	m_pChatVote(std::make_unique<ZChatVoting>())
{
	m_pAuth->SetOnAuthTokenReceivedCallback(
		[this](std::shared_ptr<ZAuthToken> p_pToken)
		{
			if (p_pToken)
			{
				m_pBroadcast = std::make_shared<ZYoutubeBroadcastConnection>(p_pToken);

				if (m_pBroadcast->IsConnected())
				{
					Logger::Info("Successfully connected to YouTube live broadcast!");

					m_pBroadcast->SetOnChatMessageCallback(
						[this](const YT::SLiveChatMessage& p_Message)
						{
							Logger::Info("Chat message from {}: {}", p_Message.m_sAuthorName, p_Message.m_sMessageText);

							m_pChatVote->PushMessage(
								p_Message.m_sAuthorId,
								p_Message.m_sMessageText
							);
						}
					);

					m_pBroadcast->SetOnPollUpdateCallback(
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
				else
				{
					Logger::Error("Failed to connect to YouTube live broadcast!");
				}
			}
		}
	);
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
		m_pAuth->StartAuthorization(false);
		Utils::CopyToClipboard(m_pAuth->GetAuthorizationUrl());
	}

	if (ImGui::Button("Disconnect"))
	{
		m_pBroadcast = nullptr;
		m_pAuth->ClearAuthToken();
	}

	if (m_pAuth->IsAuthorized())
	{
		ImGui::TextUnformatted(fmt::format("Authorized for the next {} s",
			m_pAuth->GetAuthToken()->GetRemainingValiditySeconds()
		).c_str());

		if (ImGui::Button("Refresh Token"))
		{
			m_pAuth->GetAuthToken()->Refresh(true);
		}

		if (ImGui::Button("Enable Auto-Refresh"))
		{
			m_pAuth->GetAuthToken()->StartAutoRefresh();
		}

		if (ImGui::Button("Disable Auto-Refresh"))
		{
			m_pAuth->GetAuthToken()->StopAutoRefresh();
		}
	}

	if (m_pBroadcast && m_pBroadcast->IsConnected())
	{
		ImGui::TextUnformatted(fmt::format("Connected to Stream {}",
			m_pBroadcast->GetBroadcastInfo().m_sTitle
		).c_str());
	}

	ImGui::BeginDisabled(!m_pBroadcast || !m_pBroadcast->IsConnected());

	static YT::SLivePollDetails s_PollDetails;
	if (ImGui::Button("create poll"))
	{
		s_PollDetails.m_sQuestionText = "Hello from CPP!";
		s_PollDetails.m_aOptions = {
			{ "Hello!", 0 },
			{ "Hi There!", 0 },
			{ "Howdy!", 0 },
		};
		m_pBroadcast->CreateLivePoll(s_PollDetails);
	}

	if (ImGui::Button("end poll"))
	{
		m_pBroadcast->EndLivePoll(s_PollDetails);

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

		m_pBroadcast->SendChatMessage(s_Message);
	}

	if (ImGui::Button("start chat vote"))
	{
		m_pChatVote->StartVoting({
			{ "Option 1" },
			{ "Option 2" },
			{ "Option 3" },
		});
	}

	if (ImGui::Button("end chat vote"))
	{
		m_pChatVote->EndVoting();
	}

	if (m_pChatVote->IsVotingActive())
	{
		const auto s_aVotes = m_pChatVote->GetVotes();
		const auto s_nTotalVotes = m_pChatVote->GetTotalVotes();

		for (const auto& s_Option : s_aVotes)
		{
			float s_fFraction = s_Option.m_nVoteCount / static_cast<float>(s_nTotalVotes > 0 ? s_nTotalVotes : 1);
			ImGui::TextUnformatted(fmt::format("{}: {} votes ({:.1f}%)",
				s_Option.m_sName,
				s_Option.m_nVoteCount,
				s_fFraction * 100.0f
			).c_str());
		}
	}

	ImGui::EndDisabled();
}

void ZYoutubeVotingIntegration::DrawOverlayUI()
{
	/*
	if (!m_pChatVote->IsVotingActive())
	{
		ImGui::Text("No active vote.");
		return;
	}

	const auto s_aVotes = m_pChatVote->GetVotes();
	const auto s_nTotalVotes = m_pChatVote->GetTotalVotes();

	int i = 1;
	for (const auto& s_Vote : s_aVotes)
	{
		const auto s_pEffect = m_aActiveVote[i - 1];
		const int s_nVoteCount = s_Vote.m_nVoteCount;

		const float s_fPercentage = (s_nTotalVotes > 0) 
			? (static_cast<float>(s_nVoteCount) / s_nTotalVotes) 
			: 0.0f;

		const auto s_sText = fmt::format(
			"[{}] {} ({} votes)",
			i,
			s_pEffect->GetDisplayName(true),
			s_nVoteCount
		);
		ImGuiEx::ProgressBarTextFit(s_fPercentage, s_sText.c_str());

		i++;
	}

	ImGui::TextWrapped(fmt::format("Vote by typing the option's number (1-{}) in chat!", 
		m_aActiveVote.size()
	).c_str());
	ImGui::TextDisabled(fmt::format("Total votes: {}", s_nTotalVotes).c_str());
	*/
}

REGISTER_VOTING_INTEGRATION(ZYoutubeVotingIntegration);
