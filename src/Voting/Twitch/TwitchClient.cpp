#include "TwitchClient.h"

#include "Logging.h"

#include <ixwebsocket/IXHttpServer.h>
#include <ixwebsocket/IXHttpClient.h>
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <shellapi.h>

#define TAG "[TwitchClient] "

using json = nlohmann::json;

TwitchClient::TwitchClient(const std::string p_sClientId, const int p_nServerPort)
    : m_sClientId(p_sClientId), 
    m_nServerPort(p_nServerPort)
{
    m_EventSub.SetOnChatMessageCallback([this](const std::string& username, const std::string& message) {
        OnChatMessage(username, message);
    });
}

TwitchClient::~TwitchClient()
{
    Disconnect();
}

void TwitchClient::StartAuthorization(const bool p_bOpenBrowser)
{
    // Stop any existing connections
    Disconnect();

    // Start the HTTP server in a background thread
    m_ServerThread = std::thread(&TwitchClient::RunServer, this);

    // Open the URL in the default browser
    if (p_bOpenBrowser)
    {
        std::string s_sAuthUrl = GetAuthorizationUrl();
        Logger::Info(TAG "Opening Twitch authorization URL: {}", s_sAuthUrl);
        ShellExecuteA(nullptr, "open", s_sAuthUrl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }
}

std::string TwitchClient::GetAuthorizationUrl() const
{
    // Build the authorization URL
    // We need user:read:chat scope for EventSub chat messages
    std::string s_sAuthUrl = "https://id.twitch.tv/oauth2/authorize";
    s_sAuthUrl += "?client_id=";
    s_sAuthUrl += m_sClientId;
    s_sAuthUrl += "&redirect_uri=";
    s_sAuthUrl += "http%3A%2F%2Flocalhost%3A" + std::to_string(m_nServerPort);
    s_sAuthUrl += "&response_type=token";
    s_sAuthUrl += "&scope=user%3Aread%3Achat";

    return s_sAuthUrl;
}

void TwitchClient::StopServer()
{
    {
        std::lock_guard s_Lock(m_ServerMutex);

        if (m_pServer)
        {
            m_pServer->stop();
            m_pServer.reset();
        }
    }

    if (m_ServerThread.joinable())
    {
        m_ServerThread.join();
    }
}

void TwitchClient::Disconnect()
{
    StopServer();
    m_EventSub.Disconnect();

    std::lock_guard s_Lock(m_Mutex);
    m_sAccessToken.clear();
    m_sUserId.clear();
    m_sUsername.clear();
}

bool TwitchClient::IsAuthenticated() const
{
    std::lock_guard s_Lock(m_Mutex);
    return !m_sAccessToken.empty() && !m_sUserId.empty();
}

bool TwitchClient::IsConnectedForVoting() const
{
    return IsAuthenticated() && m_EventSub.IsConnected();
}

std::string TwitchClient::GetUsername() const
{
    std::lock_guard s_Lock(m_Mutex);
    return m_sUsername;
}

void TwitchClient::StartVoting(int p_nOptionCount)
{
    m_Voting.StartVoting(p_nOptionCount);
}

std::vector<int> TwitchClient::GetVoteCounts() const
{
    return m_Voting.GetVoteCounts();
}

bool TwitchClient::IsVotingActive() const
{
    return m_Voting.IsVotingActive();
}

void TwitchClient::OnChatMessage(const std::string& p_sUsername, const std::string& p_sMessage)
{
    if (m_Voting.IsVotingActive())
    {
        m_Voting.ProcessChatMessage(p_sUsername, p_sMessage);
    }
}

void TwitchClient::RunServer()
{
    bool s_bTokenReceived = false;

    {
        std::lock_guard s_Lock(m_ServerMutex);

        if (m_pServer)
            return;

        m_pServer = std::make_unique<ix::HttpServer>(m_nServerPort, "127.0.0.1");

        m_pServer->setOnConnectionCallback(
            [this, &s_bTokenReceived](ix::HttpRequestPtr p_Request, std::shared_ptr<ix::ConnectionState>) -> ix::HttpResponsePtr
            {
                Logger::Debug(TAG "HTTP request: {} {}", p_Request->method, p_Request->uri);

                // Serve the token capture page at the root
                if (p_Request->uri == "/")
                {
                    Logger::Debug(TAG "Serving token capture page");
                    return std::make_shared<ix::HttpResponse>(
                        200, "OK",
                        ix::HttpErrorCode::Ok,
                        ix::WebSocketHttpHeaders{{"Content-Type", "text/html"}},
                        GetTokenCapturePage()
                    );
                }

                // Handle token exchange at /token and return 404 for anything else
                if (p_Request->uri.rfind("/token", 0) != 0)
                {
                    return std::make_shared<ix::HttpResponse>(
                        404, "Not Found",
                        ix::HttpErrorCode::Ok,
                        ix::WebSocketHttpHeaders{{"Content-Type", "text/plain"}},
                        "Not Found"
                    );
                }

                // Parse query string to get access_token
                std::string s_sUri = p_Request->uri;
                std::string s_sToken;

                size_t s_nTokenPos = s_sUri.find("access_token=");
                if (s_nTokenPos != std::string::npos)
                {
                    s_nTokenPos += 13; // length of "access_token="
                    size_t s_nEndPos = s_sUri.find('&', s_nTokenPos);
                    if (s_nEndPos == std::string::npos)
                        s_nEndPos = s_sUri.length();
                    s_sToken = s_sUri.substr(s_nTokenPos, s_nEndPos - s_nTokenPos);
                }

                if (s_sToken.empty())
                {
                    return std::make_shared<ix::HttpResponse>(
                        400, "Bad Request",
                        ix::HttpErrorCode::Ok,
                        ix::WebSocketHttpHeaders{{"Content-Type", "text/plain"}},
                        "Missing access_token parameter"
                    );
                }

                Logger::Info(TAG "Received access token");

                {
                    std::lock_guard s_Lock(m_Mutex);
                    m_sAccessToken = s_sToken;
                }

                // Validate token and get user info
                if (!ValidateTokenAndGetUserInfo())
                {
                    return std::make_shared<ix::HttpResponse>(
                        200, "OK",
                        ix::HttpErrorCode::Ok,
                        ix::WebSocketHttpHeaders{{"Content-Type", "text/html"}},
                        "<html><body><h1>Failed to validate token</h1></body></html>"
                    );
                }

                // Connect to EventSub for chat messages
                ConnectToEventSub();

                // Signal that we received the token (server will be stopped from main thread)
                s_bTokenReceived = true;

                return std::make_shared<ix::HttpResponse>(
                    200, "OK",
                    ix::HttpErrorCode::Ok,
                    ix::WebSocketHttpHeaders{{"Content-Type", "text/html"}},
                    GetSuccessPage());
            });

        Logger::Debug(TAG "Starting HTTP server on port {}", m_nServerPort);

        const auto s_Res = m_pServer->listen();
        if (!s_Res.first)
        {
            Logger::Error(TAG "Failed to start HTTP server: {}", s_Res.second);
            m_pServer.reset();
            return;
        }

        m_pServer->start();
    }

    // Wait for token to be received, then stop the server
    while (!s_bTokenReceived)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check if server was stopped externally
        std::lock_guard s_Lock(m_ServerMutex);
        if (!m_pServer)
            break;
    }

    // Stop the server after receiving the token
    {
        std::lock_guard s_Lock(m_ServerMutex);
        if (m_pServer)
        {
            m_pServer->stop();
            m_pServer.reset();
        }
    }

    Logger::Debug(TAG "HTTP server stopped");
}

std::string TwitchClient::GetTokenCapturePage()
{
    return R"html(
<!DOCTYPE html>
<html>
<head>
    <title>ZHM Chaos Mod - Twitch Authorization</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; background: #1a1a2e; color: #eee; }
        .spinner { border: 4px solid #333; border-top: 4px solid #9146FF; border-radius: 50%; width: 40px; height: 40px; animation: spin 1s linear infinite; margin: 20px auto; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
    </style>
</head>
<body>
    <h1>Connecting to Twitch...</h1>
    <div class="spinner"></div>
    <p>Please wait while we complete the authorization.</p>
    <script>
        const hash = window.location.hash.substring(1);
        const params = new URLSearchParams(hash);
        const accessToken = params.get('access_token');
        if (accessToken) {
            window.location.href = '/token?access_token=' + encodeURIComponent(accessToken);
        } else {
            document.body.innerHTML = '<h1>Authorization Failed</h1><p>No access token received. Please try again.</p>';
        }
    </script>
</body>
</html>
)html";
}

std::string TwitchClient::GetSuccessPage()
{
    return R"html(
<!DOCTYPE html>
<html>
<head>
    <title>ZHM Chaos Mod - Connected!</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; background: #1a1a2e; color: #eee; }
        .success { color: #4CAF50; font-size: 48px; }
    </style>
</head>
<body>
    <div class="success">&#10004;</div>
    <h1>Connected to Twitch!</h1>
    <p>You can close this window and return to the game.</p>
    <p>Twitch chat voting is now available.</p>
</body>
</html>
)html";
}

bool TwitchClient::ValidateTokenAndGetUserInfo()
{
    std::string s_sAccessToken;
    {
        std::lock_guard s_Lock(m_Mutex);
        s_sAccessToken = m_sAccessToken;
    }

    ix::HttpClient s_Client;
    ix::HttpRequestArgsPtr s_Args = s_Client.createRequest();

    // Set headers
    s_Args->extraHeaders["Authorization"] = "Bearer " + s_sAccessToken;
    s_Args->extraHeaders["Client-Id"] = m_sClientId;

    const auto s_Response = s_Client.get("https://api.twitch.tv/helix/users", s_Args);

    if (s_Response->statusCode != 200)
    {
        Logger::Error(TAG "Failed to validate token: {} - {}", s_Response->statusCode, s_Response->body);
        return false;
    }

    try
    {
        const auto s_Json = json::parse(s_Response->body);
        if (s_Json.contains("data") && !s_Json["data"].empty())
        {
            const auto& s_User = s_Json["data"][0];

            std::lock_guard s_Lock(m_Mutex);
            m_sUserId = s_User["id"].get<std::string>();
            m_sUsername = s_User["login"].get<std::string>();

            Logger::Info(TAG "Authenticated as {} (ID: {})", m_sUsername, m_sUserId);
            return true;
        }
    }
    catch (const json::exception& e)
    {
        Logger::Error(TAG "Failed to parse user info: {}", e.what());
    }

    return false;
}

void TwitchClient::ConnectToEventSub()
{
    std::string s_sAccessToken;
    std::string s_sUserId;

    {
        std::lock_guard s_Lock(m_Mutex);
        s_sAccessToken = m_sAccessToken;
        s_sUserId = m_sUserId;
    }

    if (s_sUserId.empty() || s_sAccessToken.empty())
    {
        Logger::Error(TAG "Cannot connect to EventSub: missing user ID or access token");
        return;
    }

    m_EventSub.Connect(s_sAccessToken, s_sUserId, m_sClientId);
}
