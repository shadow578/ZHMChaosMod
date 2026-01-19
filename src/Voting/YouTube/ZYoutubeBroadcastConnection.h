#pragma once
#include <string>
#include <functional>

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

	/**
	 * Set callback for new chat messages.
	 */
	void SetOnChatMessageCallback(const std::function<void(const YT::SLiveChatMessage&)>& p_Callback)
	{
		std::lock_guard s_Lock(m_ChatPollingCallbacksMutex);
		m_OnChatMessageCallback = p_Callback;
	}

	/**
	 * Set callback for live poll updates.
	 */
	void SetOnPollUpdateCallback(const std::function<void(const YT::SLivePollDetails&)>& p_Callback)
	{
		std::lock_guard s_Lock(m_ChatPollingCallbacksMutex);
		m_OnPollUpdateCallback = p_Callback;
	}

	/**
	 * Create a new live poll in the active broadcast.
	 */
	bool CreateLivePoll(const YT::SLivePollDetails& p_PollDetails);

	/**
	 * End the currently active live poll, created by CreateLivePoll.
	 */
	bool EndLivePoll();

private: // Setup and common
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

private: // Live chat polling
	mutable std::recursive_mutex m_ChatPollingCallbacksMutex;
	std::function<void(const YT::SLiveChatMessage&)> m_OnChatMessageCallback;
	std::function<void(const YT::SLivePollDetails&)> m_OnPollUpdateCallback;
	std::thread m_ChatPollingThread;

	void RunLiveChatPolling();
	int GetLiveChatMessages(std::string& p_sPageToken);
};