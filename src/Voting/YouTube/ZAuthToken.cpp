#include "ZAuthToken.h"

#include <Logging.h>

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpServer.h>
#include <ixwebsocket/IXHttpClient.h>

#include "Helpers/Utils.h"

#define TAG "[ZAuthToken] "

using json = nlohmann::json;

std::shared_ptr<ZAuthToken> ZAuthToken::FromAuthCode(const std::string& p_sClientId, const std::string& p_sAuthCode, const std::string& p_sRedirectUri)
{
	const json s_RequestJson = {
		{"code", p_sAuthCode},
		{"client_id", p_sClientId},
		{"redirect_uri", p_sRedirectUri},
		{"grant_type", "authorization_code"}
	};

	ix::HttpClient s_Client;
	ix::HttpRequestArgsPtr s_pRequest = s_Client.createRequest();

	s_pRequest->extraHeaders["Content-Type"] = "application/json";
	s_pRequest->extraHeaders["Accept"] = "application/json";

	const auto s_pResponse = s_Client.post(
		"https://oauth2.googleapis.com/token",
		s_RequestJson.dump(),
		s_pRequest
	);

	if (!s_pResponse)
	{
		Logger::Error(TAG "OAuth token exchange request failed!");
		return nullptr;
	}

	if (s_pResponse->statusCode < 200 || s_pResponse->statusCode >= 300)
	{
		Logger::Error(TAG "OAuth token exchange failed: {} {}", s_pResponse->statusCode, s_pResponse->body);
		return nullptr;
	}

	const auto s_ResponseJson = json::parse(s_pResponse->body);

	// only accept bearer token
	YT::SAuthToken s_Token = YT::SAuthToken::FromJson(s_ResponseJson);
	if (!s_Token)
	{
		Logger::Error(TAG "Invalid OAuth token response: {}", s_pResponse->body);
		return nullptr;
	}

	Logger::Debug(TAG "Got access token for scope {}. Expiration in {} seconds", s_Token.m_sScope, s_Token.m_nExpiresIn);
	return std::make_shared<ZAuthToken>(p_sClientId, s_Token);
}

ix::HttpRequestArgsPtr ZAuthToken::CreateRequestArgs(ix::HttpClient& p_Client) const
{
	auto s_pRequest = p_Client.createRequest();

	s_pRequest->extraHeaders["Authorization"] = "Bearer " + m_Token.m_sAccessToken;

	return s_pRequest;
}

uint64_t ZAuthToken::GetRemainingValiditySeconds() const
{
	const uint64_t nElapsedSeconds = Utils::GetTimestamp() - m_nTokenObtainedAt;
	if (nElapsedSeconds >= static_cast<uint64_t>(m_Token.m_nExpiresIn))
	{
		return 0;
	}

	return static_cast<uint64_t>(m_Token.m_nExpiresIn) - nElapsedSeconds;
}

void ZAuthToken::SetToken(const YT::SAuthToken& p_Token)
{
	m_Token = p_Token;
	m_nTokenObtainedAt = Utils::GetTimestamp();
}
