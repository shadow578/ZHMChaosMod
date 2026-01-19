#include "ZYoutubeBroadcastConnection.h"

#include <Logging.h>

#include <nlohmann/json.hpp>

#include "Helpers/Net/UrlUtils.h"

#define TAG "[ZYoutubeBroadcastConnection] "

using json = nlohmann::json;

ZYoutubeBroadcastConnection::ZYoutubeBroadcastConnection(const std::string p_sClientId, const YT::SAuthToken p_Token)
	: m_sClientId(p_sClientId),
	m_Token(p_Token),
	m_bReadOnly(p_Token.m_sScope == "https://www.googleapis.com/auth/youtube.readonly")
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
	if (m_ChatPollingThread.joinable())
	{
		m_ChatPollingThread.join();
	}

	m_ActiveBroadcast = {};
}

YT::SLiveBroadcast ZYoutubeBroadcastConnection::GetActiveBroadcast()
{
	ix::HttpClient s_Client;
	ix::HttpRequestArgsPtr s_pRequest = s_Client.createRequest();
	AddCommonHeaders(s_pRequest);

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

void ZYoutubeBroadcastConnection::AddCommonHeaders(ix::HttpRequestArgsPtr p_pRequest)
{
	p_pRequest->extraHeaders["Authorization"] = "Bearer " + m_Token.m_sAccessToken;
	p_pRequest->extraHeaders["Accept"] = "application/json";
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

bool ZYoutubeBroadcastConnection::CreateLivePoll(const YT::SLivePollDetails& p_PollDetails)
{
	return false;
}

bool ZYoutubeBroadcastConnection::EndLivePoll()
{
	return false;
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
	ix::HttpClient s_Client;
	ix::HttpRequestArgsPtr s_pRequest = s_Client.createRequest();
	AddCommonHeaders(s_pRequest);

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
