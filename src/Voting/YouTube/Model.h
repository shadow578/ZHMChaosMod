#pragma once
#include <string>

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
}
