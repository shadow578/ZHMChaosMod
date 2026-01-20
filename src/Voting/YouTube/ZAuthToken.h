#pragma once
#include "Model.h"

#include <string>
#include <memory>

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpClient.h>

using json = nlohmann::json;

namespace YT
{
	constexpr auto c_sReadOnlyScope = "https://www.googleapis.com/auth/youtube.readonly";
	constexpr auto c_sReadWriteScope = "https://www.googleapis.com/auth/youtube";
}

class ZAuthToken
{
public:
	/**
	 * Create auth token from OAuth2 authorization code.
	 */
	static std::shared_ptr<ZAuthToken> FromAuthCode(const std::string& p_sClientId, const std::string& p_sAuthCode, const std::string& p_sRedirectUri);

	ZAuthToken(const std::string& p_sClientId, const YT::SAuthToken& p_Token)
		: m_sClientId(p_sClientId)
	{
		SetToken(p_Token);
	}

	/**
	 * Is the token valid?
	 */
	operator bool() const
	{
		return !m_sClientId.empty() && m_Token;
	}

	/**
	 * Is the token read-only?
	 */
	bool IsReadOnly() const
	{
		return m_Token.m_sScope == YT::c_sReadOnlyScope;
	}

	/**
	 * Create an HTTP request with the appropriate authorization headers.
	 */
	ix::HttpRequestArgsPtr CreateRequestArgs(ix::HttpClient& p_Client) const;

	/**
	 * Get the number of seconds remaining until the token expires.
	 */
	uint64_t GetRemainingValiditySeconds() const;

private:
	const std::string m_sClientId;
	YT::SAuthToken m_Token;
	uint64_t m_nTokenObtainedAt;

	void SetToken(const YT::SAuthToken& p_Token);
};
