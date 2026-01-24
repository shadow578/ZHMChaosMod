#include "UrlUtils.h"

#include <sstream>
#include <iomanip>

std::string UrlUtils::URLEncode(const std::string& p_sComponent)
{
	std::ostringstream s_Out;
	s_Out << std::uppercase << std::hex;

	for (char s_cChar : p_sComponent)
	{
		// take as-is?
		if (
			std::isalnum(s_cChar) // A-Z a-z 0-9
			|| s_cChar == '-'      // allowed special chars 
			|| s_cChar == '_'
			|| s_cChar == '~'
			|| s_cChar == '~'
		)
		{
			s_Out << s_cChar;
			continue;
		}

		// encode
		s_Out << "%" << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(s_cChar));
	}

	return s_Out.str();
}

std::string UrlUtils::BuildQueryParams(const std::vector<std::pair<std::string, std::string>>& m_aParams)
{
	std::string s_sQuery;

	bool s_bFirst = true;
	for (const auto& [s_sParam, s_sValue] : m_aParams)
	{
		if (!s_bFirst)
		{
			s_sQuery += "&";
		}
		s_bFirst = false;

		s_sQuery += URLEncode(s_sParam) + "=" + URLEncode(s_sValue);
	}

	return s_sQuery;
}

