#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>

/**
 * Handles vote tallying for YouTube chat voting.
 * Tracks votes per user to prevent duplicate voting.
 */
class ZChatVoting
{
public:
    struct SVoteOption
    {
        SVoteOption(const std::string& p_sName) : m_sName(p_sName) {}

        std::string m_sName;
        int m_nVoteCount = 0;
    };

    ZChatVoting() = default;
    ~ZChatVoting() = default;

    /**
     * Start a new voting round with the given options.
     * Clears all previous votes.
     */
    void StartVoting(const std::vector<SVoteOption>& p_aOptions);

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
     * @param p_sUserId Unique identifier of the voter (ID, username, etc.)
     * @param p_sMessage The chat message
     * @return true if a valid vote was registered
     */
    bool PushMessage(const std::string& p_sUserId, const std::string& p_sMessage);

    /**
     * Get the current vote counts for each option.
     * @return Vector of nOptionCount vote counts, indexed by option (0-based)
     */
    std::vector<SVoteOption> GetVotes() const;

    /**
     * Get the total number of votes cast.
     */
    int GetTotalVotes() const;

private:
    std::atomic<bool> m_bVotingActive{false};

    mutable std::recursive_mutex m_VotesMutex; // Lock for m_aVoteCounts and m_mUserVotes
    std::vector<SVoteOption> m_aVoteCounts;
    std::unordered_map<std::string, int> m_mUserVotes;

    /**
     * Parse the vote option from a chat message.
     * @param p_sMessage The chat message
     * @return The option number (index to m_aVoteCounts) or -1 if invalid
     */
    static int ParseVoteOption(const std::string& p_sMessage);

    /**
     * Get the existing vote of a user.
     * @param p_sUserId Unique identifier of the voter
     * @return The option number (index to m_aVoteCounts) or -1 if no existing vote
     */
    int GetExistingVote(const std::string& p_sUserId) const;
};