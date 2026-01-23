#include "ZYoutubePollVotingIntegration.h"

#include "Client/ZYoutubeAuthHandler.h"
#include "Client/ZYoutubeBroadcastConnection.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/ZBackgroundWorker.h"

#define TAG "[ZYoutubePollVotingIntegration] "

ZYoutubePollVotingIntegration::ZYoutubePollVotingIntegration()
	: ZYoutubeIntegrationBase(),
	m_pWorker(std::make_unique<ZBackgroundWorker>())
{
}

void ZYoutubePollVotingIntegration::OnBroadcastConnected()
{
	if (!m_pCurrentBroadcast)
	{
		return;
	}

	// store poll updates
	m_pCurrentBroadcast->SetOnPollUpdateCallback(
		[this](const YT::SLivePollDetails& p_Poll)
		{
			std::lock_guard s_Lock(m_PollDetailsMutex);
			m_CurrentPollDetails = p_Poll;

		}
	);
}

void ZYoutubePollVotingIntegration::StartVoteImpl()
{
	m_pWorker->Enqueue(
		[this]()
		{
			m_bVoteActive = false;

			if (!m_pCurrentBroadcast || !m_pCurrentBroadcast->IsConnected())
			{
				return;
			}

			// end any existing poll first
			YT::SLivePollDetails s_ExistingPoll;
			{
				std::lock_guard s_Lock(m_PollDetailsMutex);
				s_ExistingPoll = m_CurrentPollDetails;

				m_CurrentPollDetails = {};
			}

			if (s_ExistingPoll)
			{
				Logger::Debug(TAG "Ending existing YouTube live poll before starting a new one");
				if (!m_pCurrentBroadcast->EndLivePoll(s_ExistingPoll))
				{
					Logger::Debug(TAG "Failed to end existing YouTube live poll");
				}
			}

			// start new poll
			YT::SLivePollDetails s_Poll;
			s_Poll.m_sQuestionText = "Vote for the next effect!";

			s_Poll.m_aOptions.clear();
			s_Poll.m_aOptions.reserve(m_aActiveVote.size());
			for (const auto* s_pEffect : m_aActiveVote)
			{
				s_Poll.m_aOptions.push_back({ s_pEffect->GetDisplayName(true) });

				if (s_Poll.m_aOptions.size() >= YT::c_nMaxLivePollOptions)
				{
					break;
				}
			}

			if (m_pCurrentBroadcast->CreateLivePoll(s_Poll))
			{
				Logger::Debug(TAG "Started YouTube live poll with {} options", s_Poll.m_aOptions.size());

				{
					std::lock_guard s_Lock(m_PollDetailsMutex);
					m_CurrentPollDetails = s_Poll;
				}

				m_bVoteActive = true;
			}
			else
			{
				Logger::Debug(TAG "Failed to start YouTube live poll");
			}
		}
	);

}

IChaosEffect* ZYoutubePollVotingIntegration::EndVoteImpl()
{
	// capture whatever the last known poll details were
	YT::SLivePollDetails s_FinalPollDetails;
	{
		std::lock_guard s_Lock(m_PollDetailsMutex);
		s_FinalPollDetails = m_CurrentPollDetails;

		m_CurrentPollDetails = {};
	}

	m_bVoteActive = false;

	// attempt to end the live poll asynchronously
	m_pWorker->Enqueue(
		[this, s_FinalPollDetails]()
		{
			if (!m_pCurrentBroadcast || !m_pCurrentBroadcast->IsConnected())
			{
				return;
			}

			auto s_PollToEnd = s_FinalPollDetails;

			m_pCurrentBroadcast->EndLivePoll(s_PollToEnd);
			Logger::Debug(TAG "Ended YouTube live poll async");
		}
	);

	// no poll?
	if (!s_FinalPollDetails)
	{
		Logger::Debug(TAG "No active YouTube live poll found at voting end");
		return nullptr;
	}

	// vote options vs effects count mismatch?
	if (m_aActiveVote.size() != s_FinalPollDetails.m_aOptions.size())
	{
		Logger::Debug(TAG "Vote options count ({}) does not match effects count ({})",
			s_FinalPollDetails.m_aOptions.size(),
			m_aActiveVote.size()
		);
		return nullptr;
	}

	// no votes?
	const auto s_nTotalVotes = s_FinalPollDetails.GetTotalVotes();
	if (s_nTotalVotes <= 0)
	{
		Logger::Debug(TAG "No votes were cast in YouTube live poll");
		return nullptr;
	}

	// collect winning vote count, then collect all tied options
	const auto s_nWinningVoteCountIt = std::max_element(s_FinalPollDetails.m_aOptions.begin(), s_FinalPollDetails.m_aOptions.end(),
		[](const YT::SLivePollOption& s_Lhs, const YT::SLivePollOption& s_Rhs)
		{
			return s_Lhs.m_nVoteCount < s_Rhs.m_nVoteCount;
		}
	);
	const auto s_nWinningVoteCount = s_nWinningVoteCountIt != s_FinalPollDetails.m_aOptions.end() ? s_nWinningVoteCountIt->m_nVoteCount : -1;
	if (s_nWinningVoteCount <= 0)
	{
		return nullptr;
	}

	std::vector<size_t> s_aWinningOptionIdx;
	for (size_t i = 0; i < s_FinalPollDetails.m_aOptions.size(); i++)
	{
		if (s_FinalPollDetails.m_aOptions[i].m_nVoteCount == s_nWinningVoteCount)
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

void ZYoutubePollVotingIntegration::DrawConfigUI()
{
	ZYoutubeIntegrationBase::DrawConfigUI();

	ImGui::Checkbox("Show Votes in Overlay", &m_bShowVoteCountsInOverlay);

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip("Show number of votes in the game overlay.");
	}
}

void ZYoutubePollVotingIntegration::DrawOverlayUI()
{
	if (!m_pCurrentBroadcast || !m_pCurrentBroadcast->IsConnected())
	{
		ImGui::Text("Not connected to a YouTube broadcast.");
		return;
	}

	if (!m_bVoteActive)
	{
		ImGui::Text("Preparing next vote.");
		return;
	}

	// use whatever the last known poll details were
	YT::SLivePollDetails s_Poll;
	{
		std::lock_guard s_Lock(m_PollDetailsMutex);
		s_Poll = m_CurrentPollDetails;
	}

	if (!s_Poll)
	{
		ImGui::Text("Preparing next vote.");
		return;
	}

	const auto s_nTotalVotes = s_Poll.GetTotalVotes();

	if (m_bShowVoteCountsInOverlay)
	{
		for (const auto& s_Option : s_Poll.m_aOptions)
		{
			const int s_nVoteCount = s_Option.m_nVoteCount;

			const float s_fPercentage = (s_nTotalVotes > 0)
				? (static_cast<float>(s_nVoteCount) / s_nTotalVotes)
				: 0.0f;

			const auto s_sText = fmt::format(
				"{} ({} votes)",
				s_Option.m_sOptionText,
				s_nVoteCount
			);
			ImGuiEx::ProgressBarTextFit(s_fPercentage, s_sText.c_str());
		}
	}

	ImGui::TextWrapped("Vote using the live poll!");
	ImGui::TextDisabled(fmt::format("Total votes: {}", s_nTotalVotes).c_str());
}

REGISTER_VOTING_INTEGRATION(ZYoutubePollVotingIntegration);
