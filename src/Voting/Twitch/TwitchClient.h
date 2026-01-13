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
class TwitchClient
{
public:
    TwitchClient(const std::string p_sClientId, const int p_nServerPort = 6969);
    ~TwitchClient();

    TwitchClient(const TwitchClient&) = delete;
    TwitchClient& operator=(const TwitchClient&) = delete;

    /**
     * Start the OAuth authorization flow.
     * Opens the Twitch authorization URL in the user's browser and starts
     * a local HTTP server to capture the token.
	 * @param p_bOpenBrowser Whether to open the browser automatically.
     */
    void StartAuthorization(const bool p_bOpenBrowser);

    /**
     * Get the Twitch authorization URL to open in the browser.
     * This does not start the server or open the browser.
	 */
	std::string GetAuthorizationUrl() const;

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
    const std::string m_sClientId;
	const int m_nServerPort;

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

