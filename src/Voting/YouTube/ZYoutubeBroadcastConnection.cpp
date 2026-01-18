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

	return true;
}

void ZYoutubeBroadcastConnection::Disconnect()
{
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
		const auto s_Status = s_Item.value("status", json::object());
		const auto s_Snippet = s_Item.value("snippet", json::object());

		const auto s_sId = s_Item.value("id", "");
		const auto s_sTitle = s_Snippet.value("title", "");
		const auto s_sLiveChatId = s_Snippet.value("liveChatId", "");
		const auto s_sBroadcastStatus = s_Status.value("lifeCycleStatus", "");

		Logger::Debug(TAG "Got active broadcast: id='{}', title='{}', liveChatId='{}', status='{}'",
			s_sId,
			s_sTitle,
			s_sLiveChatId,
			s_sBroadcastStatus
		);

		if (s_sBroadcastStatus == "live")
		{
			return YT::SLiveBroadcast{
				s_sId,
				s_sTitle,
				s_sLiveChatId
			};
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
