#pragma once

#include <string>
#include <memory>
#include <mutex>

namespace ix { class HttpServer; }

struct SAuthToken
{
	std::string m_sAccessToken;
	std::string m_sRefreshToken;
	int m_nExpiresIn;
};

class ZYoutubeApiConnection
{
public:
	ZYoutubeApiConnection(const std::string p_sClientId, const SAuthToken p_Token)
		: m_sClientId(p_sClientId),
		m_Token(p_Token)
	{

	}

	bool IsConnected() const { return false;  }



private:
	const std::string m_sClientId;
	SAuthToken m_Token;

};

class ZYoutubeClient
{
public:
	ZYoutubeClient(const std::string p_sClientId, const bool p_bReadOnly, const int p_nTokenCapturePort = 6967);
	~ZYoutubeClient();

	ZYoutubeClient(const ZYoutubeClient&) = delete;
	ZYoutubeClient& operator=(const ZYoutubeClient&) = delete;

	/**
	 * Get the YouTube authorization URL to open in the browser.
	 * This does not start the server or open the browser.
	 * You must call StartAuthorization to do that.
	 */
	std::string GetAuthorizationUrl();

	/**
	 * Start the OAuth authorization flow.
	 * A local HTTP server is started to capture the token.
	 * @param p_bOpenBrowser Whether to open the browser automatically.
	 */
	void Connect(const bool p_bOpenBrowser);

	/**
	 * Disconnect from YouTube.
	 * This will stop the token capture server, and api polling threads.
	 */
	void Disconnect();

	/**
	 * Check if we have a valid access token.
	 */
	bool IsConnected() const;

private: // OAuth & token capture
	const std::string m_sClientId;
	const int m_nTokenCapturePort;
	const bool m_bReadOnlyScope;

	mutable std::recursive_mutex m_TokenCaptureServerMutex;
	std::thread m_TokenCaptureServerThread;
	std::unique_ptr<ix::HttpServer> m_pTokenCaptureServer;

	void StartTokenCaptureServer();
	void StopTokenCaptureServer();
	void RunTokenCaptureServer();

	bool GetAuthTokenFromCode(const std::string& p_sAuthCode, SAuthToken& p_Token);

private: // API Session
	mutable std::recursive_mutex m_ApiConnectionMutex;
	std::unique_ptr<ZYoutubeApiConnection> m_pApiConnection;

};