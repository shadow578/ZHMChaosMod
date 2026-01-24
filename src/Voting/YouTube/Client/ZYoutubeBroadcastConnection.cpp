#include "ZYoutubeBroadcastConnection.h"
#include "ZAuthToken.h"

#include <Logging.h>

#include <nlohmann/json.hpp>

#include "Helpers/Net/UrlUtils.h"

#define TAG "[ZYoutubeBroadcastConnection] "

using json = nlohmann::json;

ZYoutubeBroadcastConnection::ZYoutubeBroadcastConnection(const std::shared_ptr<ZAuthToken> m_pToken)
	: m_pToken(m_pToken)
{
	if (!Connect())
	{
		Logger::Error(TAG "Failed to connect to live broadcast!");
	}
}

ZYoutubeBroadcastConnection::~ZYoutubeBroadcastConnection()
{
	Disconnect();
}

bool ZYoutubeBroadcastConnection::Connect()
{
	m_ActiveBroadcast = GetActiveBroadcast();
	if (!m_ActiveBroadcast)
	{
		Logger::Error(TAG "No active live broadcast found for the authenticated user.");
		return false;
	}

	Logger::Info(TAG "Connected to live broadcast: id='{}', title='{}', liveChatId='{}'",
		m_ActiveBroadcast.m_sId,
		m_ActiveBroadcast.m_sTitle,
		m_ActiveBroadcast.m_sLiveChatId
	);

	// start polling loop
	m_ChatPollingThread = std::thread(&ZYoutubeBroadcastConnection::RunLiveChatPolling, this);
	return true;
}

void ZYoutubeBroadcastConnection::Disconnect()
{
	m_ActiveBroadcast = {};

	if (m_ChatPollingThread.joinable())
	{
		m_ChatPollingThread.join();
	}
}

YT::SLiveBroadcast ZYoutubeBroadcastConnection::GetActiveBroadcast()
{
	ix::HttpClient s_Client;
	auto s_pRequest = m_pToken->CreateRequestArgs(s_Client);

	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_pResponse = s_Client.get(
		UrlUtils::BuildQueryUrl("https://www.googleapis.com/youtube/v3/liveBroadcasts", {
			{ "part", "id,status,snippet" },
			{ "broadcastStatus", "active" },
			{ "maxResults", "1" } // assume only one active broadcast
			}),
		s_pRequest
	);

	if (!IsSuccessfulResponse(s_pResponse, "GetActiveBroadcast"))
	{
		return {};
	}

	const auto s_Json = json::parse(s_pResponse->body);

	for (auto& s_Item : s_Json.value("items", json::array()))
	{
		const auto s_Broadcast = YT::SLiveBroadcast::FromJson(s_Item);
		Logger::Debug(TAG "Got active broadcast: id='{}', title='{}', liveChatId='{}', status='{}'",
			s_Broadcast.m_sId,
			s_Broadcast.m_sTitle,
			s_Broadcast.m_sLiveChatId,
			s_Broadcast.m_sLifecycleStatus
		);

		if (s_Broadcast.m_sLifecycleStatus == "live")
		{
			return s_Broadcast;
		}
	}

	return {};
}

bool ZYoutubeBroadcastConnection::IsSuccessfulResponse(const ix::HttpResponsePtr p_pResponse, const std::string& p_sContext)
{
	if (!p_pResponse)
	{
		Logger::Error(TAG "{}: no response received!", p_sContext);
		return false;
	}

	if (p_pResponse->statusCode < 200 || p_pResponse->statusCode >= 300)
	{
		Logger::Error(TAG "{}: HTTP {}: {}", p_sContext, p_pResponse->statusCode, p_pResponse->body);
		return false;
	}

	return true;
}

bool ZYoutubeBroadcastConnection::CreateLivePoll(YT::SLivePollDetails& p_PollDetails)
{
	if (m_pToken->IsReadOnly())
	{
		Logger::Error(TAG "CreateLivePoll called on read-only connection!");
		return false;
	}

	// set livechat id from active broadcast if not provided
	if (p_PollDetails.m_sLiveChatId.empty())
	{
		p_PollDetails.m_sLiveChatId = m_ActiveBroadcast.m_sLiveChatId;
	}

	ix::HttpClient s_Client;
	auto s_pRequest = m_pToken->CreateRequestArgs(s_Client);

	s_pRequest->extraHeaders["Content-Type"] = "application/json";
	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_PollDetailsJson = YT::SLivePollDetails::ToJson(p_PollDetails);

	const auto s_pResponse = s_Client.post(
		UrlUtils::BuildQueryUrl("https://www.googleapis.com/youtube/v3/liveChat/messages", {
			{ "part", "snippet" }
		}),
		s_PollDetailsJson.dump(),
		s_pRequest
	);

	if (!IsSuccessfulResponse(s_pResponse, "CreateLivePoll"))
	{
		return false;
	}

	const auto s_Json = json::parse(s_pResponse->body);
	const auto s_FinalPollDetails = YT::SLivePollDetails::FromJson(s_Json);
	if (!s_FinalPollDetails)
	{
		return false;
	}

	p_PollDetails = s_FinalPollDetails;
	return true;
}

bool ZYoutubeBroadcastConnection::EndLivePoll(YT::SLivePollDetails& p_PollDetails)
{
	if (m_pToken->IsReadOnly())
	{
		Logger::Error(TAG "EndLivePoll called on read-only connection!");
		return false;
	}

	if (!p_PollDetails)
	{
		return false;
	}

	ix::HttpClient s_Client;
	auto s_pRequest = m_pToken->CreateRequestArgs(s_Client);

	s_pRequest->extraHeaders["Content-Type"] = "application/json";
	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_pResponse = s_Client.post(
		UrlUtils::BuildQueryUrl("https://www.googleapis.com/youtube/v3/liveChat/messages/transition", {
			{ "id", p_PollDetails.m_sId },
			{ "status", "closed" },
			{ "part", "snippet" }
		}),
		"",
		s_pRequest
	);

	if (!IsSuccessfulResponse(s_pResponse, "EndLivePoll")) // FIXME fails here
	{
		return false;
	}

	const auto s_Json = json::parse(s_pResponse->body);
	const auto s_FinalPollDetails = YT::SLivePollDetails::FromJson(s_Json);
	if (s_FinalPollDetails)
	{
		p_PollDetails = s_FinalPollDetails;
	}

	return true;
}

bool ZYoutubeBroadcastConnection::SendChatMessage(YT::SLiveChatMessage& p_Message)
{
	if (m_pToken->IsReadOnly())
	{
		Logger::Error(TAG "CreateLivePoll called on read-only connection!");
		return false;
	}

	// set livechat id from active broadcast if not provided
	if (p_Message.m_sLiveChatId.empty())
	{
		p_Message.m_sLiveChatId = m_ActiveBroadcast.m_sLiveChatId;
	}

	ix::HttpClient s_Client;
	auto s_pRequest = m_pToken->CreateRequestArgs(s_Client);

	s_pRequest->extraHeaders["Content-Type"] = "application/json";
	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_MessageDetailsJson = YT::SLiveChatMessage::ToJson(p_Message);

	const auto s_pResponse = s_Client.post(
		UrlUtils::BuildQueryUrl("https://www.googleapis.com/youtube/v3/liveChat/messages", {
			{ "part", "snippet" }
		}),
		s_MessageDetailsJson.dump(),
		s_pRequest
	);

	if (!IsSuccessfulResponse(s_pResponse, "SendChatMessage"))
	{
		return false;
	}

	const auto s_Json = json::parse(s_pResponse->body);
	const auto s_FinalMessage = YT::SLiveChatMessage::FromJson(s_Json);
	if (!s_FinalMessage)
	{
		return false;
	}

	p_Message = s_FinalMessage;
	return true;
}

void ZYoutubeBroadcastConnection::RunLiveChatPolling()
{
	std::string s_sNextPageToken = "";
	int s_nThrottleMs = 0;
	while (IsConnected())
	{
		if (s_nThrottleMs > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(s_nThrottleMs));
		}

		s_nThrottleMs = GetLiveChatMessages(s_sNextPageToken);
		if (s_nThrottleMs < 0)
		{
			Logger::Error(TAG "Live chat polling error!");
			s_nThrottleMs = 1000;
		}
	}
}

int ZYoutubeBroadcastConnection::GetLiveChatMessages(std::string& p_sPageToken)
{
	{
		std::lock_guard s_Lock(m_ChatPollingCallbacksMutex);
		if (!m_OnChatMessageCallback && !m_OnPollUpdateCallback)
		{
			// no callbacks, no need to query api
			return 500;
		}
	}

	ix::HttpClient s_Client;
	auto s_pRequest = m_pToken->CreateRequestArgs(s_Client);

	s_pRequest->extraHeaders["Accept"] = "application/json";

	std::vector<std::pair<std::string, std::string>> s_aParams = {
			{ "liveChatId", m_ActiveBroadcast.m_sLiveChatId },
			{ "part", "snippet,authorDetails" },
	};
	if (!p_sPageToken.empty())
	{
		s_aParams.push_back({ "pageToken", p_sPageToken });
	}

	const auto s_pResponse = s_Client.get(
		UrlUtils::BuildQueryUrl("https://www.googleapis.com/youtube/v3/liveChat/messages", s_aParams),
		s_pRequest
	);

	if (!IsSuccessfulResponse(s_pResponse, "GetLiveChatMessages"))
	{
		return -1;
	}

	const auto s_Json = json::parse(s_pResponse->body);

	// extract live poll details and invoke callback
	const auto s_ActivePollItem = s_Json.value("activePollItem", json::object());
	const auto s_PollDetails = YT::SLivePollDetails::FromJson(s_ActivePollItem);

	if (s_PollDetails)
	{
		std::lock_guard s_Lock(m_ChatPollingCallbacksMutex);
		if (m_OnPollUpdateCallback)
		{
			m_OnPollUpdateCallback(s_PollDetails);
		}
	}


	// extract chat messages and invoke callback
	for (const auto& s_Item : s_Json.value("items", json::array()))
	{
		YT::SLiveChatMessage s_Message;

		s_Message = YT::SLiveChatMessage::FromJson(s_Item);
		if (s_Message)
		{
			std::lock_guard s_Lock(m_ChatPollingCallbacksMutex);
			if (m_OnChatMessageCallback)
			{
				m_OnChatMessageCallback(s_Message);
			}
		}
	}

	// update next page token and throttle
	p_sPageToken = s_Json.value("nextPageToken", "");
	return s_Json.value("pollingIntervalMillis", 500);
}
