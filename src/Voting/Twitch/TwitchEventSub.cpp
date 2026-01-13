#include "TwitchEventSub.h"

#include "Logging.h"

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpClient.h>

#define TAG "[TwitchEventSub] "

using json = nlohmann::json;

TwitchEventSub::~TwitchEventSub()
{
    Disconnect();
}

void TwitchEventSub::Connect(const std::string& p_sAccessToken, const std::string& p_sUserId, const std::string& p_sClientId)
{
    std::lock_guard s_Lock(m_Mutex);

    m_sAccessToken = p_sAccessToken;
    m_sUserId = p_sUserId;
    m_sClientId = p_sClientId;

    if (m_pWebSocket)
    {
        m_pWebSocket->stop();
    }

    m_pWebSocket = std::make_unique<ix::WebSocket>();
    m_pWebSocket->setUrl(c_sEventSubUrl);

    m_pWebSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        OnMessage(msg);
    });

    Logger::Debug(TAG "Connecting to Twitch EventSub...");
    m_pWebSocket->start();
}

void TwitchEventSub::Disconnect()
{
    std::lock_guard s_Lock(m_Mutex);

    if (m_pWebSocket)
    {
        m_pWebSocket->stop();
        m_pWebSocket.reset();
    }

    m_bConnected = false;
    m_bSubscribed = false;
    m_sSessionId.clear();

    if (m_OnConnectionStateChangedCallback)
    {
        m_OnConnectionStateChangedCallback(false);
    }

    Logger::Debug(TAG "Disconnected from Twitch EventSub.");
}

bool TwitchEventSub::IsConnected() const
{
    return m_bConnected && m_bSubscribed;
}

void TwitchEventSub::SetOnChatMessageCallback(std::function<void(const std::string&, const std::string&)> p_Callback)
{
    m_OnChatMessageCallback = std::move(p_Callback);
}

void TwitchEventSub::SetOnConnectionStateChangedCallback(std::function<void(bool)> p_Callback)
{
    m_OnConnectionStateChangedCallback = std::move(p_Callback);
}

void TwitchEventSub::OnMessage(const ix::WebSocketMessagePtr& p_Msg)
{
    switch (p_Msg->type)
    {
    case ix::WebSocketMessageType::Open:
        Logger::Debug(TAG "WebSocket connection opened!");
        break;

    case ix::WebSocketMessageType::Close:
        Logger::Debug(TAG "WebSocket connection closed: {} {}", p_Msg->closeInfo.code, p_Msg->closeInfo.reason);
        m_bConnected = false;
        m_bSubscribed = false;
        if (m_OnConnectionStateChangedCallback)
        {
            m_OnConnectionStateChangedCallback(false);
        }
        break;

    case ix::WebSocketMessageType::Error:
        Logger::Error(TAG "WebSocket error: {}", p_Msg->errorInfo.reason);
        break;

    case ix::WebSocketMessageType::Message:
        {
            try
            {
                const auto s_Json = json::parse(p_Msg->str);
                const auto s_sMessageType = s_Json["metadata"]["message_type"].get<std::string>();

                if (s_sMessageType == "session_welcome")
                {
                    HandleWelcomeMessage(p_Msg->str);
                }
                else if (s_sMessageType == "session_keepalive")
                {
                    Logger::Debug(TAG "Received keepalive");
                }
                else if (s_sMessageType == "notification")
                {
                    HandleNotificationMessage(p_Msg->str);
                }
                else if (s_sMessageType == "session_reconnect")
                {
                    HandleReconnectMessage(p_Msg->str);
                }
                else if (s_sMessageType == "revocation")
                {
                    Logger::Warn(TAG "Subscription revoked");
                    // What do we even do here? Ask user to re-auth?
                }
            }
            catch (const json::exception& e)
            {
                Logger::Error(TAG "Failed to parse message: {}", e.what());
            }
        }
        break;

    default:
        break;
    }
}

void TwitchEventSub::HandleWelcomeMessage(const std::string& p_sPayload)
{
    try
    {
        const auto s_Json = json::parse(p_sPayload);
        m_sSessionId = s_Json["payload"]["session"]["id"].get<std::string>();
        m_bConnected = true;

        Logger::Info(TAG "Received welcome message, session ID: {}", m_sSessionId);

        // Now subscribe to chat messages
        SubscribeToChatMessages();
    }
    catch (const json::exception& e)
    {
        Logger::Error(TAG "Failed to parse welcome message: {}", e.what());
    }
}

void TwitchEventSub::HandleNotificationMessage(const std::string& p_sPayload)
{
    try
    {
        auto s_Json = json::parse(p_sPayload);
        const auto s_sSubType = s_Json["metadata"]["subscription_type"].get<std::string>();

        if (s_sSubType == "channel.chat.message")
        {
            const auto& s_Event = s_Json["payload"]["event"];
            const auto s_sUser = s_Event["chatter_user_login"].get<std::string>();
            const auto s_sMessage = s_Event["message"]["text"].get<std::string>();

            Logger::Debug(TAG "Chat message from {}: {}", s_sUser, s_sMessage);

            if (m_OnChatMessageCallback)
            {
                m_OnChatMessageCallback(s_sUser, s_sMessage);
            }
        }
    }
    catch (const json::exception& e)
    {
        Logger::Error(TAG "Failed to parse notification: {}", e.what());
    }
}

void TwitchEventSub::HandleReconnectMessage(const std::string& p_sPayload)
{
    try
    {
        const auto s_Json = json::parse(p_sPayload);
        const auto s_sReconnectUrl = s_Json["payload"]["session"]["reconnect_url"].get<std::string>();

        Logger::Debug(TAG "Received reconnect message, reconnecting to: {}", s_sReconnectUrl);

        // Create new WebSocket connection to the reconnect URL
        auto s_pNewWebSocket = std::make_unique<ix::WebSocket>();
        s_pNewWebSocket->setUrl(s_sReconnectUrl);
        s_pNewWebSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
            OnMessage(msg);
        });

        // Acquire lock before modifying shared state
        // Stop old socket, swap in new one, then start new one
        {
            std::lock_guard s_Lock(m_Mutex);
            if (m_pWebSocket)
            {
                m_pWebSocket->stop();
            }
            m_pWebSocket = std::move(s_pNewWebSocket);
        }

        // Start the new WebSocket outside the lock to avoid potential deadlock
        // with the callback trying to acquire the lock
        m_pWebSocket->start();
    }
    catch (const json::exception& e)
    {
        Logger::Error(TAG "Failed to parse reconnect message: {}", e.what());
    }
}

void TwitchEventSub::SubscribeToChatMessages()
{
    Logger::Info(TAG "Subscribing to chat messages for user {}", m_sUserId);

    json s_SubBody = {
        {"type", "channel.chat.message"},
        {"version", "1"},
        {"condition", {
            {"broadcaster_user_id", m_sUserId},
            {"user_id", m_sUserId}
        }},
        {"transport", {
            {"method", "websocket"},
            {"session_id", m_sSessionId}
        }}
    };

    std::string s_sResponse;
    if (MakeApiRequest("/eventsub/subscriptions", "POST", s_SubBody.dump(), s_sResponse))
    {
        Logger::Info(TAG "Successfully subscribed to chat messages!");
        m_bSubscribed = true;

        if (m_OnConnectionStateChangedCallback)
        {
            m_OnConnectionStateChangedCallback(true);
        }
    }
    else
    {
        Logger::Error(TAG "Failed to subscribe to chat messages: {}", s_sResponse);
    }
}

bool TwitchEventSub::MakeApiRequest(const std::string& p_sEndpoint, const std::string& p_sMethod,
                                     const std::string& p_sBody, std::string& p_sResponse) const {
    ix::HttpClient s_Client;
    ix::HttpRequestArgsPtr s_Args = s_Client.createRequest();

    // Set headers
    s_Args->extraHeaders["Authorization"] = "Bearer " + m_sAccessToken;
    s_Args->extraHeaders["Client-Id"] = m_sClientId;
    s_Args->extraHeaders["Content-Type"] = "application/json";

    const std::string s_sUrl = std::string(c_sTwitchApiUrl) + p_sEndpoint;

    ix::HttpResponsePtr s_Response;
    if (p_sMethod == "POST")
    {
        s_Response = s_Client.post(s_sUrl, p_sBody, s_Args);
    }
    else if (p_sMethod == "GET")
    {
        s_Response = s_Client.get(s_sUrl, s_Args);
    }
    else
    {
        Logger::Error(TAG "Unsupported HTTP method: {}", p_sMethod);
        __debugbreak();
        return false;
    }

    if (!s_Response)
    {
        p_sResponse = "HTTP request failed";
        return false;
    }

    p_sResponse = s_Response->body;

    if (s_Response->statusCode >= 200 && s_Response->statusCode < 300)
    {
        return true;
    }

    Logger::Error(TAG "API request failed with status {}: {}", s_Response->statusCode, s_Response->body);
    return false;
}

