#pragma once
#include <string>

#include <ixwebsocket/IXHttpClient.h>


#include "Model.h"

/**
 * Handles connection to the YouTube API, including live chat polling.
 */
class ZYoutubeBroadcastConnection
{
public:
	ZYoutubeBroadcastConnection(const std::string p_sClientId, const YT::SAuthToken p_Token);
	~ZYoutubeBroadcastConnection();

	ZYoutubeBroadcastConnection(const ZYoutubeBroadcastConnection&) = delete;
	ZYoutubeBroadcastConnection& operator=(const ZYoutubeBroadcastConnection&) = delete;

	/**
	 * Query the currently active live broadcast for the authenticated user, then start chat polling.
	 * Automatically called on construction.
	 */
	bool Connect();

	/**
	 * Disconnect from the YouTube API, stop chat polling.
	 * Automatically called on destruction.
	 */
	void Disconnect();

	/**
	 * Is the connection active?
	 */
	bool IsConnected() const
	{
		return m_Token && m_ActiveBroadcast;
	}

	/**
	 * Get information about the currently active live broadcast.
	 */
	YT::SLiveBroadcast GetBroadcastInfo() const
	{
		return m_ActiveBroadcast;
	}

private:
	const std::string m_sClientId;
	const YT::SAuthToken m_Token;
	const bool m_bReadOnly;

	YT::SLiveBroadcast m_ActiveBroadcast;

	/**
	 * Get the currently active live broadcast for the authenticated user.
	 */
	YT::SLiveBroadcast GetActiveBroadcast();

	void AddCommonHeaders(ix::HttpRequestArgsPtr p_pRequest);
	bool IsSuccessfulResponse(const ix::HttpResponsePtr p_pResponse, const std::string& p_sContext);
};