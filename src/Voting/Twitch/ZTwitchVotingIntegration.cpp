#include "ZTwitchVotingIntegration.h"

#include <Logging.h>
#include <IconsMaterialDesign.h>

#include <numeric>

#include "EffectRegistry.h"
#include "Helpers/Math.h"
#include "Helpers/Utils.h"
#include "Helpers/ImGuiExtras.h"

#define TAG "[ZTwitchVotingIntegration] "

ZTwitchVotingIntegration::ZTwitchVotingIntegration()
	: m_pTwitch(std::make_unique<TwitchClient>("u6vwcqu2o637hq3z1edengct7xbm5q"))
{
}

void ZTwitchVotingIntegration::StartVote(const std::vector<IChaosEffect*>& p_aOptions)
{
	m_aActiveVote = p_aOptions;

	if (m_pTwitch && m_pTwitch->IsConnectedForVoting())
	{
		m_pTwitch->StartVoting(m_aActiveVote.size());
		Logger::Debug(TAG "Started twitch voting with {} options", m_aActiveVote.size());
	}
}

IChaosEffect* ZTwitchVotingIntegration::EndVote()
{
	if (auto* s_pEffect = EndVoteTwitch())
	{
		m_aActiveVote.clear();
		return s_pEffect;
	}

	// fallback to random selection if twitch voting is inactive
	Logger::Debug(TAG "Twitch voting failed, fallback to random selection");
	if (m_aActiveVote.empty())
	{
		return nullptr;
	}

	auto* s_pEffect =  Math::SelectRandomElement(m_aActiveVote);
	m_aActiveVote.clear();
	return s_pEffect;
}

IChaosEffect* ZTwitchVotingIntegration::EndVoteTwitch()
{
	if (!m_pTwitch || !m_pTwitch->IsConnectedForVoting() || !m_pTwitch->IsVotingActive())
	{
		Logger::Debug(TAG "Started twitch voting with {} options", m_aActiveVote.size());
		return nullptr;
	}

	m_pTwitch->GetVoting().EndVoting();

	const auto& s_aWinningOptions = m_pTwitch->GetVoting().GetWinningOptions();
	if (s_aWinningOptions.empty())
	{
		return nullptr;
	}

	// select randomly among tieing votes.
	// if only one winner, SelectRandomElement can only select that one, so no further checks
	const auto s_nWinner = Math::SelectRandomElement(s_aWinningOptions);

	// out of range?!
	if (s_nWinner < 0 || s_nWinner > m_aActiveVote.size())
	{
		return nullptr;
	}

	auto* s_pEffect = m_aActiveVote[s_nWinner];
	Logger::Debug(TAG "Twitch chat voted for option #{} ({})", 
		s_nWinner,
		s_pEffect->GetName()
	);

	return s_pEffect;
}

void ZTwitchVotingIntegration::DrawConfigUI()
{
	if (!m_pTwitch)
	{
		return;
	}

	if (m_pTwitch->IsConnectedForVoting())
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ICON_MD_CHECK_CIRCLE " Connected to Twitch");

		const auto s_sUsername = m_pTwitch->GetUsername();
		if (!s_sUsername.empty())
		{
			ImGui::SameLine();
			ImGui::TextDisabled("(%s)", s_sUsername.c_str());
		}

		if (ImGui::Button(ICON_MD_LINK_OFF " Disconnect"))
		{
			m_pTwitch->Disconnect();
		}
	}
	else if (m_pTwitch->IsAuthenticated())
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), ICON_MD_HOURGLASS_EMPTY " Connecting to chat...");
	}
	else
	{
		ImGui::TextWrapped("Connect to Twitch to enable chat voting for effects.");

		if (ImGui::Button(ICON_MD_LINK " Connect to Twitch"))
		{
			m_pTwitch->StartAuthorization(true);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens your browser to authorize the mod with your Twitch account.");
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_MD_CONTENT_COPY))
		{
			m_pTwitch->StartAuthorization(false);
			Utils::CopyToClipboard(m_pTwitch->GetAuthorizationUrl());
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Copy authorization URL to your clipboard.");
		}
	}
}

void ZTwitchVotingIntegration::DrawOverlayUI()
{
	if (!m_pTwitch || !m_pTwitch->IsConnectedForVoting())
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Not connected to Twitch!");
		return;
	}

	// Abort if no active vote
	if (m_aActiveVote.size() == 0 || !m_pTwitch->GetVoting().IsVotingActive())
	{
		return;
	}

	// Get vote counts
	std::vector<int> s_aVoteCounts = m_pTwitch->GetVoteCounts();
	int s_nTotalVotes = std::accumulate(s_aVoteCounts.begin(), s_aVoteCounts.end(), 0);

	// Ensure that we have a vote count for each option
	s_aVoteCounts.resize(m_aActiveVote.size(), 0);

	// Display vote options with counts
	for (size_t i = 0; i < m_aActiveVote.size(); i++)
	{
		const auto& s_Effect = m_aActiveVote[i];

		// Show vote count and percentage
		const float s_fPercentage = s_nTotalVotes > 0 ? static_cast<float>(s_aVoteCounts[i]) / s_nTotalVotes : 0.0f;
		const auto s_sText = fmt::format(
			"[{}] {} ({} votes)",
			i + 1,
			s_Effect->GetDisplayName(true),
			s_aVoteCounts[i]
		);

		ImGuiEx::ProgressBarTextFit(s_fPercentage, s_sText.c_str());
	}

	ImGui::TextWrapped("Vote by typing the option's number (1-%d) in chat!", m_aActiveVote.size());
	ImGui::TextDisabled("Total votes: %d", s_nTotalVotes);
}

REGISTER_VOTING_INTEGRATION(ZTwitchVotingIntegration);
