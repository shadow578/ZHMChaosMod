#pragma once

#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>

#include "TwitchEventSub.h"
#include "TwitchVoting.h"

namespace ix { class HttpServer; }

/**
 * Handles Twitch OAuth authorization and integration.
 * Provides an HTTP server to capture OAuth tokens from the redirect.
 * Manages EventSub connection for chat voting.
 */
class TwitchIntegration
{
public:
    TwitchIntegration();
    ~TwitchIntegration();

    TwitchIntegration(const TwitchIntegration&) = delete;
    TwitchIntegration& operator=(const TwitchIntegration&) = delete;

    /**
     * Start the OAuth authorization flow.
     * Opens the Twitch authorization URL in the user's browser and starts
     * a local HTTP server to capture the token.
     */
    void StartAuthorization();

    /**
     * Stop the HTTP server if running.
     */
    void StopServer();

    /**
     * Disconnect from Twitch (both auth server and EventSub).
     */
    void Disconnect();

    /**
     * Check if we have a valid access token.
     */
    bool IsAuthenticated() const;

    /**
     * Check if we are connected to EventSub and ready for voting.
     */
    bool IsConnectedForVoting() const;

    /**
     * Get the username of the authenticated user.
     */
    std::string GetUsername() const;

    // Voting interface
    TwitchVoting& GetVoting() { return m_Voting; }
    const TwitchVoting& GetVoting() const { return m_Voting; }

    /**
     * Start a new voting round.
     */
    void StartVoting(int p_nOptionCount);

    /**
     * Get the current vote counts.
     */
    std::vector<int> GetVoteCounts() const;

    /**
     * Check if voting is currently active.
     */
    bool IsVotingActive() const;

private:
    static constexpr const char* c_sClientId = "u6vwcqu2o637hq3z1edengct7xbm5q";
    static constexpr int c_nServerPort = 6969;

    mutable std::recursive_mutex m_Mutex;
    std::string m_sAccessToken;
    std::string m_sUserId;
    std::string m_sUsername;

    std::thread m_ServerThread;
    mutable std::recursive_mutex m_ServerMutex;
    std::unique_ptr<ix::HttpServer> m_pServer;

    TwitchEventSub m_EventSub;
    TwitchVoting m_Voting;

    /**
     * Run the HTTP server to capture the OAuth token.
     */
    void RunServer();

    /**
     * Validate the access token and get user info.
     */
    bool ValidateTokenAndGetUserInfo();

    /**
     * Connect to EventSub after authentication.
     */
    void ConnectToEventSub();

    /**
     * Handle incoming chat messages for voting.
     */
    void OnChatMessage(const std::string& p_sUsername, const std::string& p_sMessage);

    /**
     * Generate the HTML page that extracts the token from the URL fragment.
     */
    static std::string GetTokenCapturePage();

    /**
     * Generate the success page shown after token capture.
     */
    static std::string GetSuccessPage();
};

