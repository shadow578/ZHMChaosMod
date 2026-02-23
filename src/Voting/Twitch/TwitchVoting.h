#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>

/**
 * Handles vote tallying for Twitch chat voting.
 * Tracks votes per user to prevent duplicate voting.
 */
class TwitchVoting
{
public:
    TwitchVoting() = default;
    ~TwitchVoting() = default;

    /**
     * Start a new voting round with the given number of options.
     * Clears all previous votes.
     */
    void StartVoting(int p_nOptionCount);

    /**
     * End the current voting round.
     */
    void EndVoting();

    /**
     * Check if voting is currently active.
     */
    bool IsVotingActive() const;

    /**
     * Process a chat message and extract a vote if present.
     * @param p_sUsername The username of the voter
     * @param p_sMessage The chat message
     * @return true if a valid vote was registered
     */
    bool ProcessChatMessage(const std::string& p_sUsername, const std::string& p_sMessage);

    /**
     * Get the current vote counts for each option.
     * @return Vector of vote counts, indexed by option (0-based)
     */
    std::vector<int> GetVoteCounts() const;

    /**
     * Get all options that have the highest vote count (for tie-breaking).
     */
    std::vector<int> GetWinningOptions() const;

private:
    mutable std::recursive_mutex m_Mutex;
    std::atomic<bool> m_bVotingActive{false};
    int m_nOptionCount{0};

    // Maps username to their vote (1-based option number)
    std::unordered_map<std::string, int> m_UserVotes;

    // Vote counts per option (0-based index)
    std::vector<int> m_VoteCounts;

    /**
     * Parse a vote number from a message.
     * @return Vote number (1-based) or 0 if no valid vote found
     */
    static int ParseVote(const std::string& p_sMessage);
};

