#include "ZYoutubeChatVotingIntegration.h"

#include "Client/ZYoutubeAuthHandler.h"
#include "Client/ZYoutubeBroadcastConnection.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/Voting/ZChatVoting.h"

#define TAG "[ZYoutubeChatVotingIntegration] "

ZYoutubeChatVotingIntegration::ZYoutubeChatVotingIntegration()
	: ZYoutubeIntegrationBase(),
	m_pVoting(std::make_unique<ZChatVoting>())
{
}

void ZYoutubeChatVotingIntegration::OnBroadcastConnected()
{
	if (!m_pCurrentBroadcast)
	{
		return;
	}

	// forward chat messages to voting system
	m_pCurrentBroadcast->SetOnChatMessageCallback(
		[this](const YT::SLiveChatMessage& p_Message)
		{
			if (m_bMembersOnlyVoting)
			{
				// only owner, moderator and sponsors can vote
				if (!p_Message.m_bAuthorIsOwner
					&& !p_Message.m_bAuthorIsModerator
					&& !p_Message.m_bAuthorIsSponsor)
				{
					return;
				}
			}

			m_pVoting->PushMessage(
				p_Message.m_sAuthorId,
				p_Message.m_sMessageText
			);
		}
	);
}

void ZYoutubeChatVotingIntegration::StartVoteImpl()
{
	if (m_pCurrentBroadcast && m_pCurrentBroadcast->IsConnected())
	{
		std::vector<ZChatVoting::SVoteOption> s_nOptions;
		s_nOptions.reserve(m_aActiveVote.size());
		for (const auto* s_pEffect : m_aActiveVote)
		{
			s_nOptions.push_back({ s_pEffect->GetName() });
		}

		m_pVoting->StartVoting(s_nOptions);
		Logger::Debug(TAG "Started YouTube chat voting with {} options", s_nOptions.size());
	}
}

IChaosEffect* ZYoutubeChatVotingIntegration::EndVoteImpl()
{
	if (!m_pVoting->IsVotingActive())
	{
		Logger::Debug(TAG "YouTube chat voting is not active");
		return nullptr;
	}

	m_pVoting->EndVoting();
	const auto& s_aFinalTally = m_pVoting->GetVotes();

	// no votes?
	if (m_pVoting->GetTotalVotes() <= 0)
	{
		Logger::Debug(TAG "No votes were cast in YouTube chat voting");
		return nullptr;
	}

	// collect winning vote count, then collect all tied options
	const auto s_nWinningVoteCountIt = std::max_element(s_aFinalTally.begin(), s_aFinalTally.end(),
		[](const ZChatVoting::SVoteOption& s_Lhs, const ZChatVoting::SVoteOption& s_Rhs)
		{
			return s_Lhs.m_nVoteCount < s_Rhs.m_nVoteCount;
		}
	);
	const auto s_nWinningVoteCount = s_nWinningVoteCountIt != s_aFinalTally.end() ? s_nWinningVoteCountIt->m_nVoteCount : -1;
	if (s_nWinningVoteCount <= 0)
	{
		return nullptr;
	}

	std::vector<size_t> s_aWinningOptionIdx;
	for (size_t i = 0; i < s_aFinalTally.size(); i++)
	{
		if (s_aFinalTally[i].m_nVoteCount == s_nWinningVoteCount)
		{
			s_aWinningOptionIdx.push_back(i);
		}
	}

	// select randomly among tieing votes to break ties
	const auto s_nWinnerIdx = Math::SelectRandomElement(s_aWinningOptionIdx);
	if (s_nWinnerIdx < 0 || s_nWinnerIdx >= m_aActiveVote.size())
	{
		return nullptr;
	}

	return m_aActiveVote[s_nWinnerIdx];
}

void ZYoutubeChatVotingIntegration::DrawConfigUI()
{
	ZYoutubeIntegrationBase::DrawConfigUI();

	ImGui::Checkbox("Members-only voting", &m_bMembersOnlyVoting);

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Only allow channel members (sponsors), moderators, and the channel owner to vote.");
	}
}

void ZYoutubeChatVotingIntegration::DrawOverlayUI()
{
	if (!m_pVoting->IsVotingActive())
	{
		ImGui::Text("No active vote.");
		return;
	}

	const auto s_aVotes = m_pVoting->GetVotes();
	const auto s_nTotalVotes = m_pVoting->GetTotalVotes();

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
}

REGISTER_VOTING_INTEGRATION(ZYoutubeChatVotingIntegration);
