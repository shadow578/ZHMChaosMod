#include "ZChatVoting.h"

#include <Logging.h>

#include <algorithm>
#include <numeric>

#define TAG "[ZChatVoting] "

void ZChatVoting::StartVoting(const std::vector<SVoteOption>& p_aOptions)
{
    std::lock_guard s_Lock(m_VotesMutex);

    m_aVoteCounts = p_aOptions;
    for (auto& option : m_aVoteCounts)
    {
        option.m_nVoteCount = 0;
    }

    m_mUserVotes.clear();
    m_bVotingActive = true;

    Logger::Debug(TAG "Started voting for {} options", m_aVoteCounts.size());
}

void ZChatVoting::EndVoting()
{
    m_bVotingActive = false;
}

bool ZChatVoting::IsVotingActive() const
{
    return m_bVotingActive;
}

bool ZChatVoting::PushMessage(const std::string& p_sUserId, const std::string& p_sMessage)
{
    if (!m_bVotingActive)
    {
        return false;
    }

    std::lock_guard s_Lock(m_VotesMutex);

    const auto s_nVote = ParseVoteOption(p_sMessage);
    if (s_nVote < 0 // invalid? 
        || s_nVote >= m_aVoteCounts.size()) // out of range?
    {
        return false;
    }

    // changed vote?
    const auto s_nPreviousVote = GetExistingVote(p_sUserId);
    if (s_nPreviousVote >= 0 && s_nPreviousVote != s_nVote)
    {
        if (s_nPreviousVote < m_aVoteCounts.size())
        {
            m_aVoteCounts[s_nPreviousVote].m_nVoteCount--;
            if (m_aVoteCounts[s_nPreviousVote].m_nVoteCount < 0)
            {
                m_aVoteCounts[s_nPreviousVote].m_nVoteCount = 0;
            }
        }
    }

    m_aVoteCounts[s_nVote].m_nVoteCount++;
    m_mUserVotes[p_sUserId] = s_nVote;

    Logger::Debug(TAG "{} voted for option {} (previously {})", 
        p_sUserId, 
        s_nVote, 
        s_nPreviousVote
    );
    return true;
}

std::vector<ZChatVoting::SVoteOption> ZChatVoting::GetVotes() const
{
    std::lock_guard s_Lock(m_VotesMutex);
    return m_aVoteCounts;
}

int ZChatVoting::GetTotalVotes() const
{
    std::lock_guard s_Lock(m_VotesMutex);
    return std::accumulate(
        m_aVoteCounts.begin(),
        m_aVoteCounts.end(),
        0,
        [](int p_nAcc, const SVoteOption& p_Option)
        {
            return p_nAcc + p_Option.m_nVoteCount;
        }
    );
}

int ZChatVoting::GetExistingVote(const std::string& p_sUserId) const
{
    auto it = m_mUserVotes.find(p_sUserId);
    if (it != m_mUserVotes.end())
    {
        return it->second;
    }

    return -1;
}

int ZChatVoting::ParseVoteOption(const std::string& p_sMessage)
{
    std::string s_sTrimmed = p_sMessage;

    const auto s_nStart = s_sTrimmed.find_first_not_of(" \t\r\n");
    if (s_nStart == std::string::npos)
    {
        return -1;
    }

    s_sTrimmed = s_sTrimmed.substr(s_nStart);
    if (s_sTrimmed.empty())
    {
        return -1;
    }

    // look for a single digit 1-9 at the start
    // additionally, handle a hashtag followed by a number
    auto s_cVote = s_sTrimmed[0];
    if (s_cVote == '#' && s_sTrimmed.length() > 1)
    {
        s_cVote = s_sTrimmed[1];
    }
    
    if (!std::isdigit(s_cVote))
    {
        return -1;
    }

    const auto s_nVote = s_cVote - '0';
    if (s_nVote < 1 || s_nVote > 9)
    {
        return -1;
    }

    return s_nVote - 1; // convert to 0-based index
}
