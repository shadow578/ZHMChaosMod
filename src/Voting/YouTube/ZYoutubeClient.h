#pragma once
#include <string>
#include <memory>
#include <mutex>

#include "Model.h"
#include "ZYoutubeBroadcastConnection.h"

namespace ix { class HttpServer; }

/**
 * Handles connecting to YouTube API via OAuth2.
 * After connecting, provides an API session that handles requests and chat polling.
 * 
 * Notice: 
 * This class only supports OAuth2 authorization code flow WITHOUT a client secret.
 * Google doesn't make clear how to create such an OAuth client, so follow these steps:
 * - create a new OAuth client in Google Cloud Console, using the "Universal Windows Platform (UWP)" application type.
 * - Google will provide you a client id and secret. Ignore the secret, it is not needed for UWP (and only UWP!).
 * - ensure you've added the Youtube Data API v3 to the project
 * - ensure the redirect URI "http://localhost:<port>" is allowed. Port must match the one used in the constructor.
 */
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

	/**
	 * Get the currently active broadcast connection.
	 * nullptr if not connected.
	 */
	ZYoutubeBroadcastConnection* GetBroadcastConnection();

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

	bool GetAuthTokenFromCode(const std::string& p_sAuthCode, YT::SAuthToken& p_Token);

private: // API Session
	mutable std::recursive_mutex m_BroadcastConnectionMutex;
	std::unique_ptr<ZYoutubeBroadcastConnection> m_pBroadcastConnection;
};