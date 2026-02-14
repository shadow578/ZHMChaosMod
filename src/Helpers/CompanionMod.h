#pragma once

#include <string>

namespace CompanionModUtil
{
	struct SVersion
	{
		int m_nMajor;
		int m_nMinor;
		int m_nPatch;

		bool operator>=(const SVersion& other) const
		{
			if (m_nMajor != other.m_nMajor)
				return m_nMajor > other.m_nMajor;
			if (m_nMinor != other.m_nMinor)
				return m_nMinor > other.m_nMinor;
			return m_nPatch >= other.m_nPatch;
		}

		bool operator<(const SVersion& other) const
		{
			return !(*this >= other);
		}
	};

	struct SCompanionModMetadata
	{
		bool m_bPresent;
		std::string m_sVersion;

		SVersion m_Version;
	};

	/**
	 * Load metadata about the companion mod.
	 * It is recommended to call this function once in resource loading.
	 * The result is cached automatically for subsequent calls.
	 */
	SCompanionModMetadata LoadCompanionModInfo(const bool p_bAllowLoad = true);

	/**
	 * Check if the companion mod is present.
	 */
	bool IsCompanionModPresent();
}
