#pragma once
#include <string>
#include <vector>

namespace YT
{
	struct SAuthToken
	{
		std::string m_sScope;
		std::string m_sAccessToken;
		std::string m_sRefreshToken;
		int m_nExpiresIn;

		operator bool() const
		{
			return !m_sAccessToken.empty();
		}
	};

	struct SLiveBroadcast
	{
		std::string m_sId;
		std::string m_sTitle;
		std::string m_sLiveChatId;

		operator bool() const
		{
			return !m_sId.empty();
		}
	};

	struct SLiveChatMessage
	{
		std::string s_sAuthorId;
		std::string s_sAuthorName;
		std::string s_sMessageText;

		operator bool() const
		{
			return !s_sAuthorId.empty() && !s_sAuthorName.empty() && !s_sMessageText.empty();
		}
	};

	struct SLivePollOption
	{
		std::string s_sOptionText;
		int s_nVoteCount;
	};

	struct SLivePollDetails
	{
		std::string s_sQuestionText;
		std::vector<SLivePollOption> s_aOptions;
	};
}
