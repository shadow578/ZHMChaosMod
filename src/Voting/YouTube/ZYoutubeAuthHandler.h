#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <functional>
#include <thread>

class ZAuthToken;

namespace ix { class HttpServer; }

/**
 * Handles authentication and api client creation for YouTube API.
 * 
 * Notice: 
 * This class only supports OAuth2 authorization code flow WITHOUT a client secret.
 * Google doesn't make clear how to create such an OAuth client, so follow these steps:
 * - create a new OAuth client in Google Cloud Console, using the "Universal Windows Platform (UWP)" application type.
 * - Google will provide you a client id and secret. Ignore the secret, it is not needed for UWP (and only UWP!).
 * - ensure you've added the Youtube Data API v3 to the project
 * - ensure the redirect URI "http://localhost:<port>" is allowed. Port must match the one used in the constructor.
 */
class ZYoutubeAuthHandler
{
public:
	ZYoutubeAuthHandler(const std::string p_sClientId, const bool p_bReadOnly, const int p_nTokenCapturePort = 6967);
	~ZYoutubeAuthHandler();

	ZYoutubeAuthHandler(const ZYoutubeAuthHandler&) = delete;
	ZYoutubeAuthHandler& operator=(const ZYoutubeAuthHandler&) = delete;

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
	void StartAuthorization(const bool p_bOpenBrowser);

	/**
	 * Stop the authorization flow and the local HTTP server.
	 */
	void StopAuthorization();

	/**
	 * Clear the acquired auth token.
	 */
	void ClearAuthToken()
	{
		std::lock_guard s_Lock(m_AuthTokenMutex);
		m_pAuthToken = nullptr;
	}

	/**
	 * Get the acquired auth token.
	 * nullptr if no token acquired yet.
	 */
	std::shared_ptr<ZAuthToken> GetAuthToken();

	/**
	 * Check if we have acquired an auth token.
	 */
	bool IsAuthorized() const
	{
		std::lock_guard s_Lock(m_AuthTokenMutex);
		return m_pAuthToken != nullptr;
	}

	void SetOnAuthTokenReceivedCallback(const std::function<void(std::shared_ptr<ZAuthToken>)>& p_Callback)
	{
		std::lock_guard s_Lock(m_OnAuthTokenReceivedCallbackMutex);
		m_OnAuthTokenReceivedCallback = p_Callback;
	}

private:
	const std::string m_sClientId;
	const std::string m_sApiScope;
	const int m_nTokenCapturePort;

	mutable std::recursive_mutex m_TokenCaptureServerMutex;
	std::thread m_TokenCaptureServerThread;
	std::unique_ptr<ix::HttpServer> m_pTokenCaptureServer;

	mutable std::recursive_mutex m_AuthTokenMutex;
	std::shared_ptr<ZAuthToken> m_pAuthToken;

	mutable std::recursive_mutex m_OnAuthTokenReceivedCallbackMutex;
	std::function<void(std::shared_ptr<ZAuthToken>)> m_OnAuthTokenReceivedCallback;

	void StartTokenCaptureServer();
	void StopTokenCaptureServer();
	void RunTokenCaptureServer();

	void SetAuthToken(std::shared_ptr<ZAuthToken> p_pToken);
};
