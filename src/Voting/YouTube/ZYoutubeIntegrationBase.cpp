#include "ZYoutubeIntegrationBase.h"

#include <Logging.h>
#include <IconsMaterialDesign.h>

#include "Client/ZYoutubeAuthHandler.h"
#include "Client/ZYoutubeBroadcastConnection.h"
#include "Client/ZAuthToken.h"

#include "Helpers/Utils.h"
#include "Helpers/Math.h"

#define TAG "[ZYoutubeIntegrationBase] "

ZYoutubeIntegrationBase::ZYoutubeIntegrationBase(const bool p_bReadOnly)
	: m_pYoutubeAuth(std::make_unique<ZYoutubeAuthHandler>("200073485619-fc2jvhk4rsqve7lfv93s888al922phbi.apps.googleusercontent.com", p_bReadOnly)),
	m_pCurrentBroadcast(nullptr)
{
	m_pYoutubeAuth->SetOnAuthTokenReceivedCallback(
		[this](std::shared_ptr<ZAuthToken> p_pToken)
		{
			this->OnAuthTokenReceived(p_pToken);
		}
	);
}

void ZYoutubeIntegrationBase::OnAuthTokenReceived(std::shared_ptr<ZAuthToken> p_pToken)
{
	if (!p_pToken)
	{
		m_pCurrentBroadcast = nullptr;
		return;
	}

	m_pCurrentBroadcast = std::make_shared<ZYoutubeBroadcastConnection>(p_pToken);
	if (m_pCurrentBroadcast && m_pCurrentBroadcast->IsConnected())
	{
		Logger::Debug(TAG "Connected to broadcast {}", m_pCurrentBroadcast->GetBroadcastInfo().m_sTitle);
		OnBroadcastConnected();
	}
	else 
	{
		Logger::Warn(TAG "Failed to connect to active broadcast. Is there a currently active live stream?");
	}
}

void ZYoutubeIntegrationBase::Deactivate()
{
	Disconnect();
}

void ZYoutubeIntegrationBase::DrawConfigUI()
{
	if (!m_pYoutubeAuth)
	{
		return;
	}

	ImGui::TextDisabled("YouTube Integration is experimental. Please report any issues you encounter.");

	if (m_pYoutubeAuth->IsAuthorized())
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), ICON_MD_CHECK_CIRCLE " Connected to YouTube");

		ImGui::SameLine();
		if (m_pCurrentBroadcast && m_pCurrentBroadcast->IsConnected())
		{
			ImGui::TextDisabled("(%s)", m_pCurrentBroadcast->GetBroadcastInfo().m_sTitle.c_str());
		}
		else
		{
			ImGui::TextDisabled("(No active broadcast)");
		}

		if (ImGui::Button(ICON_MD_LINK_OFF " Disconnect"))
		{
			Disconnect();
		}
	}
	else
	{
		ImGui::TextWrapped("Connect to YouTube to enable viewers voting for effects.");

		if (ImGui::Button(ICON_MD_LINK " Connect to YouTube"))
		{
			m_pYoutubeAuth->StartAuthorization(true);
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Opens your browser to authorize the mod with your Google account.");
		}

		ImGui::SameLine();

		if (ImGui::Button(ICON_MD_CONTENT_COPY))
		{
			m_pYoutubeAuth->StartAuthorization(false);
			Utils::CopyToClipboard(m_pYoutubeAuth->GetAuthorizationUrl());
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Copy authorization URL to your clipboard.");
		}
	}
}

void ZYoutubeIntegrationBase::StartVote(const std::vector<IChaosEffect*>& p_aOptions)
{
	m_aActiveVote = p_aOptions;
	StartVoteImpl();
}

IChaosEffect* ZYoutubeIntegrationBase::EndVote()
{
	if (auto* s_pEffect = EndVoteImpl())
	{
		m_aActiveVote.clear();
		return s_pEffect;
	}

	// fallback to random selection
	Logger::Debug(TAG "Voting failed, fallback to random selection");
	if (m_aActiveVote.empty())
	{
		return nullptr;
	}

	auto* s_pEffect = Math::SelectRandomElement(m_aActiveVote);
	m_aActiveVote.clear();
	return s_pEffect;
}

void ZYoutubeIntegrationBase::Disconnect()
{
	m_pYoutubeAuth->ClearAuthToken();
	m_pCurrentBroadcast = nullptr;
}
