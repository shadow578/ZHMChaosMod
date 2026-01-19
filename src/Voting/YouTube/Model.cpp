#include "Model.h"

YT::SAuthToken YT::SAuthToken::FromJson(const json& p_Json)
{
	const auto s_sType = p_Json.value("token_type", "");
	if (s_sType != "Bearer")
	{
		return {};
	}

	YT::SAuthToken s_Token;
	s_Token.m_sScope = p_Json.value("scope", "");
	s_Token.m_sAccessToken = p_Json.value("access_token", "");
	s_Token.m_sRefreshToken = p_Json.value("refresh_token", "");
	s_Token.m_nExpiresIn = p_Json.value("expires_in", 0);
	return s_Token;
}

YT::SLiveBroadcast YT::SLiveBroadcast::FromJson(const json& p_Json)
{
	const auto s_Status = p_Json.value("status", json::object());
	const auto s_Snippet = p_Json.value("snippet", json::object());

	YT::SLiveBroadcast s_Broadcast;
	s_Broadcast.m_sId = p_Json.value("id", "");
	s_Broadcast.m_sTitle = s_Snippet.value("title", "");
	s_Broadcast.m_sLiveChatId = s_Snippet.value("liveChatId", "");
	s_Broadcast.m_sLifecycleStatus = s_Status.value("lifeCycleStatus", "");
	return s_Broadcast;
}

YT::SLiveChatMessage YT::SLiveChatMessage::FromJson(const json& p_Json)
{
	const auto s_Snippet = p_Json.value("snippet", json::object());
	const auto s_AuthorDetails = p_Json.value("authorDetails", json::object());

	if (s_Snippet.value("type", "") != "textMessageEvent")
	{
		return {};
	}

	YT::SLiveChatMessage s_Message;

	s_Message.m_sAuthorId = s_AuthorDetails.value("channelId", "");
	s_Message.m_sAuthorName = s_AuthorDetails.value("displayName", "");

	s_Message.m_sMessageText = s_Snippet.value("textMessageDetails", json::object())
		.value("messageText", "");

	return s_Message;
}

YT::SLivePollOption YT::SLivePollOption::FromJson(const json& p_Json)
{
	YT::SLivePollOption s_Option;

	s_Option.m_sOptionText = p_Json.value("optionText", "");

	// note: youtube, in their infinite wisdom, returns vote counts as strings
	const auto s_sVoteCount = p_Json.value("tally", "0");
	s_Option.m_nVoteCount = std::stoi(s_sVoteCount);

	return s_Option;
}

YT::SLivePollDetails YT::SLivePollDetails::FromJson(const json& p_Json)
{
	const auto s_Snippet = p_Json.value("snippet", json::object());
	const auto s_Metadata = s_Snippet.value("pollDetails", json::object())
		.value("metadata", json::object());

	if (s_Snippet.value("type", "") != "pollEvent")
	{
		return {};
	}

	SLivePollDetails s_PollDetails;
	s_PollDetails.m_sQuestionText = s_Metadata.value("questionText", "");

	for (const auto& s_OptionJson : s_Metadata.value("options", json::array()))
	{
		const auto s_Option = YT::SLivePollOption::FromJson(s_OptionJson);
		if (s_Option)
		{
			s_PollDetails.m_aOptions.push_back(s_Option);
		}
	}

	return s_PollDetails;	
}

