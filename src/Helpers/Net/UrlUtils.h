#pragma once

#include <string>
#include <vector>

namespace UrlUtils
{
	/**
	 * URL-encode a string 
	 */
	std::string URLEncode(const std::string& p_sComponent);

	/**
	 * Build query parameters from a list of parameters. 
	 * May contain duplicate keys
	 */
	std::string BuildQueryParams(const std::vector<std::pair<std::string, std::string>>& m_aParams);

	/**
	 * Build query url given base url and parameters.
	 * Parameters may contain duplicate keys
	 */
	inline std::string BuildQueryUrl(const std::string& p_sBaseUrl, const std::vector<std::pair<std::string, std::string>>& m_aParams)
	{
		return p_sBaseUrl + "?" + BuildQueryParams(m_aParams);
	}
};
