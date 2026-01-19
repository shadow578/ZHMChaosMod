#pragma once
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace YT
{
	struct SAuthToken
	{
		std::string m_sScope;		 // .scope
		std::string m_sAccessToken;  // .access_token
		std::string m_sRefreshToken; // .refresh_token
		int m_nExpiresIn;			 // .expires_in

		operator bool() const
		{
			return !m_sAccessToken.empty();
		}

		static SAuthToken FromJson(const json& p_Json);
	};

	struct SLiveBroadcast
	{
		std::string m_sId;				// .id
		std::string m_sTitle;			// .snippet.title
		std::string m_sLiveChatId;		// .snippet.liveChatId
		std::string m_sLifecycleStatus;	// .status.lifeCycleStatus

		operator bool() const
		{
			return !m_sId.empty();
		}

		static SLiveBroadcast FromJson(const json& p_Json);
	};

	struct SLiveChatMessage
	{
		std::string m_sAuthorId;	// .authorDetails.channelId
		std::string m_sAuthorName;  // .authorDetails.displayName
		std::string m_sMessageText; // .snippet.textMessageDetails.messageText

		operator bool() const
		{
			return !m_sAuthorId.empty() && !m_sAuthorName.empty() && !m_sMessageText.empty();
		}

		static SLiveChatMessage FromJson(const json& p_Json);
	};

	struct SLivePollOption
	{
		std::string m_sOptionText;	// .optionText
		int m_nVoteCount;			// .tally

		operator bool() const
		{
			return !m_sOptionText.empty();
		}

		static SLivePollOption FromJson(const json& p_Json);
	};

	struct SLivePollDetails
	{
		std::string m_sQuestionText;			 // .snippet.pollDetails.metadata.questionText
		std::vector<SLivePollOption> m_aOptions; // .snippet.pollDetails.metadata.options[]

		operator bool() const
		{
			return !m_aOptions.empty();
		}

		static SLivePollDetails FromJson(const json& p_Json);
	};
}
