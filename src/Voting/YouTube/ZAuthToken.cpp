#include "ZAuthToken.h"

#include <Logging.h>

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpServer.h>
#include <ixwebsocket/IXHttpClient.h>

#include "Helpers/Utils.h"

#define TAG "[ZAuthToken] "

using json = nlohmann::json;

constexpr uint64_t c_nTokenRefreshThresholdSeconds = 5 * 60; // 5 minutes

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

	YT::SAuthToken s_Token = YT::SAuthToken::FromJson(s_ResponseJson);
	if (!s_Token)
	{
		Logger::Error(TAG "Invalid OAuth token response: {}", s_pResponse->body);
		return nullptr;
	}

	return std::make_shared<ZAuthToken>(p_sClientId, s_Token);
}

ix::HttpRequestArgsPtr ZAuthToken::CreateRequestArgs(ix::HttpClient& p_Client) const
{
	std::lock_guard s_Lock(m_TokenMutex);

	auto s_pRequest = p_Client.createRequest();

	s_pRequest->extraHeaders["Authorization"] = "Bearer " + m_Token.m_sAccessToken;

	return s_pRequest;
}

uint64_t ZAuthToken::GetRemainingValiditySeconds() const
{
	std::lock_guard s_Lock(m_TokenMutex);

	const uint64_t nElapsedSeconds = Utils::GetTimestamp() - m_nTokenObtainedAt;
	if (nElapsedSeconds >= static_cast<uint64_t>(m_Token.m_nExpiresIn))
	{
		return 0;
	}

	return static_cast<uint64_t>(m_Token.m_nExpiresIn) - nElapsedSeconds;
}

void ZAuthToken::Refresh(const bool p_bForce)
{
	if (!p_bForce && GetRemainingValiditySeconds() > c_nTokenRefreshThresholdSeconds)
	{
		return;
	}

	if (!RefreshToken())
	{
		Logger::Error(TAG "Failed to refresh auth token!");
	}
}

void ZAuthToken::SetToken(const YT::SAuthToken& p_Token, const bool p_bIsRefresh)
{
	Logger::Debug(TAG "Got access token for scope {}. Expiration in {} seconds", p_Token.m_sScope, p_Token.m_nExpiresIn);

	std::lock_guard s_Lock(m_TokenMutex);

	m_Token.m_sScope = p_Token.m_sScope;
	m_Token.m_sAccessToken = p_Token.m_sAccessToken;
	m_Token.m_nExpiresIn = p_Token.m_nExpiresIn;

	// token refresh does not return a new refresh token.
	// if one is provided anyway, use it.
	if (!p_Token.m_sRefreshToken.empty())
	{
		m_Token.m_sRefreshToken = p_Token.m_sRefreshToken;
	}
	else
	{
		// log error if this is not a refresh operation
		if (!p_bIsRefresh)
		{
			Logger::Error(TAG "No refresh token provided in auth token response!");
		}
	}

	m_nTokenObtainedAt = Utils::GetTimestamp();
}

bool ZAuthToken::RefreshToken()
{
	Logger::Debug(TAG "Starting token refresh");

	std::string s_sRefreshToken;
	{
		std::lock_guard s_Lock(m_TokenMutex);
		s_sRefreshToken = m_Token.m_sRefreshToken;
	}

	if (s_sRefreshToken.empty())
	{
		Logger::Error(TAG "No refresh token set!");
		return false;
	}

	const json s_RequestJson = {
		{"refresh_token", s_sRefreshToken},
		{"client_id", m_sClientId},
		{"grant_type", "refresh_token"}
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
		Logger::Error(TAG "OAuth token refresh request failed!");
		return false;
	}

	if (s_pResponse->statusCode < 200 || s_pResponse->statusCode >= 300)
	{
		Logger::Error(TAG "OAuth token refresh failed: {} {}", s_pResponse->statusCode, s_pResponse->body);
		return false;
	}

	const auto s_ResponseJson = json::parse(s_pResponse->body);

	// only accept bearer token
	YT::SAuthToken s_Token = YT::SAuthToken::FromJson(s_ResponseJson);
	if (!s_Token)
	{
		Logger::Error(TAG "Invalid OAuth token response after refresh: {}", s_pResponse->body);
		return false;
	}

	SetToken(s_Token, true);
	return true;
}
