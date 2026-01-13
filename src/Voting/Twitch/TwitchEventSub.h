#pragma once

#include <string>
#include <atomic>
#include <functional>
#include <mutex>
#include <memory>

#include <ixwebsocket/IXWebSocket.h>

/**
 * Handles Twitch EventSub WebSocket connection for receiving chat messages.
 * Used for Twitch chat voting integration.
 */
class TwitchEventSub
{
public:
    TwitchEventSub() = default;
    ~TwitchEventSub();

    TwitchEventSub(const TwitchEventSub&) = delete;
    TwitchEventSub& operator=(const TwitchEventSub&) = delete;

    /**
     * Connect to Twitch EventSub WebSocket.
     * @param p_sAccessToken The OAuth access token
     * @param p_sUserId The user ID of the broadcaster
     * @param p_sClientId The client ID of the application
     */
    void Connect(const std::string& p_sAccessToken, const std::string& p_sUserId, const std::string& p_sClientId);

    /**
     * Disconnect from the WebSocket.
     */
    void Disconnect();

    /**
     * Check if connected to EventSub.
     */
    bool IsConnected() const;

    /**
     * Set callback for when a chat message is received.
     * @param p_Callback Function that receives (username, message)
     */
    void SetOnChatMessageCallback(std::function<void(const std::string&, const std::string&)> p_Callback);

    /**
     * Set callback for when connection state changes.
     */
    void SetOnConnectionStateChangedCallback(std::function<void(bool)> p_Callback);

private:
    static constexpr const char* c_sEventSubUrl = "wss://eventsub.wss.twitch.tv/ws";
    static constexpr const char* c_sTwitchApiUrl = "https://api.twitch.tv/helix";

    std::unique_ptr<ix::WebSocket> m_pWebSocket;
    std::string m_sSessionId;
    std::string m_sAccessToken;
    std::string m_sUserId;
    std::string m_sClientId;

    std::atomic<bool> m_bConnected{false};
    std::atomic<bool> m_bSubscribed{false};
    mutable std::recursive_mutex m_Mutex;

    std::function<void(const std::string&, const std::string&)> m_OnChatMessageCallback;
    std::function<void(bool)> m_OnConnectionStateChangedCallback;

    /**
     * Handle incoming WebSocket messages.
     */
    void OnMessage(const ix::WebSocketMessagePtr& p_Msg);

    /**
     * Handle the welcome message and extract session ID.
     */
    void HandleWelcomeMessage(const std::string& p_sPayload);

    /**
     * Handle notification messages (chat events).
     */
    void HandleNotificationMessage(const std::string& p_sPayload);

    /**
     * Handle reconnect messages.
     */
    void HandleReconnectMessage(const std::string& p_sPayload);

    /**
     * Subscribe to channel.chat.message events.
     */
    void SubscribeToChatMessages();

    /**
     * Make an HTTP request to the Twitch API.
     */
    bool MakeApiRequest(const std::string& p_sEndpoint, const std::string& p_sMethod, 
                        const std::string& p_sBody, std::string& p_sResponse) const;
};

