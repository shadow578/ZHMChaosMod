#include "TwitchVoting.h"

#include "Logging.h"

#include <algorithm>
#include <numeric>

#define TAG "[TwitchVoting] "

void TwitchVoting::StartVoting(int p_nOptionCount)
{
    std::lock_guard s_Lock(m_Mutex);

    m_nOptionCount = p_nOptionCount;
    m_UserVotes.clear();
    m_VoteCounts.clear();
    m_VoteCounts.resize(p_nOptionCount, 0);
    m_bVotingActive = true;

    Logger::Debug(TAG "Started voting with {} options", p_nOptionCount);
}

void TwitchVoting::EndVoting()
{
    int s_nTotalVotes = 0;

    {
        std::lock_guard s_Lock(m_Mutex);
        m_bVotingActive = false;
        s_nTotalVotes = std::accumulate(m_VoteCounts.begin(), m_VoteCounts.end(), 0);
    }

    Logger::Debug(TAG "Ended voting. Total votes: {}", s_nTotalVotes);
}

bool TwitchVoting::IsVotingActive() const
{
    return m_bVotingActive;
}

bool TwitchVoting::ProcessChatMessage(const std::string& p_sUsername, const std::string& p_sMessage)
{
    if (!m_bVotingActive)
    {
        return false;
    }

    const int s_nVote = ParseVote(p_sMessage);
    if (s_nVote <= 0)
    {
        return false;
    }

    std::lock_guard s_Lock(m_Mutex);

    // Check vote is within valid range (must be done under lock)
    if (s_nVote > m_nOptionCount)
    {
        return false;
    }

    // Check if user already voted
    auto it = m_UserVotes.find(p_sUsername);
    if (it != m_UserVotes.end())
    {
        int s_nOldVote = it->second;
        if (s_nOldVote == s_nVote)
        {
            // Same vote, ignore
            return false;
        }

        // Remove old vote
        if (s_nOldVote > 0 && s_nOldVote <= static_cast<int>(m_VoteCounts.size()))
        {
            m_VoteCounts[s_nOldVote - 1]--;
        }

        Logger::Debug(TAG "User {} changed vote from {} to {}", p_sUsername, s_nOldVote, s_nVote);
    }
    else
    {
        Logger::Debug(TAG "User {} voted for option {}", p_sUsername, s_nVote);
    }

    // Record the new vote
    m_UserVotes[p_sUsername] = s_nVote;
    m_VoteCounts[s_nVote - 1]++;

    return true;
}

std::vector<int> TwitchVoting::GetVoteCounts() const
{
    std::lock_guard s_Lock(m_Mutex);
    return m_VoteCounts;
}

std::vector<int> TwitchVoting::GetWinningOptions() const
{
    std::lock_guard s_Lock(m_Mutex);

    if (m_VoteCounts.empty())
    {
        return {};
    }

    const auto s_nMaxVotes = *std::ranges::max_element(m_VoteCounts);
    if (s_nMaxVotes == 0)
    {
        return {};
    }

    std::vector<int> s_aWinners;
    for (int i = 0; i < static_cast<int>(m_VoteCounts.size()); i++)
    {
        if (m_VoteCounts[i] == s_nMaxVotes)
        {
            s_aWinners.push_back(i);
        }
    }

    return s_aWinners;
}

int TwitchVoting::ParseVote(const std::string& p_sMessage) {
    // Trim whitespace and look for a single digit 1-9 at the start
    std::string s_sTrimmed = p_sMessage;

    const auto s_nStart = s_sTrimmed.find_first_not_of(" \t\r\n");
    if (s_nStart == std::string::npos)
    {
        return 0;
    }

    s_sTrimmed = s_sTrimmed.substr(s_nStart);

    // Check if the message starts with a digit
    if (!s_sTrimmed.empty() && std::isdigit(s_sTrimmed[0]))
    {
        const auto s_nVote = s_sTrimmed[0] - '0';

        // Ensure it's a valid vote (1-9) and either end of string or followed by non-digit
        if (s_nVote >= 1 && s_nVote <= 9)
        {
            if (s_sTrimmed.length() == 1 || !std::isdigit(s_sTrimmed[1]))
            {
                return s_nVote;
            }
        }
    }

    return 0;
}

