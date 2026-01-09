// Build information, variables auto-generated during build process in BuildInfo.cpp
// note: any changes here must be reflected in gen-build-info.cmake
#pragma once
#include <string>

namespace BuildInfo
{
	extern const std::string c_sTargetSDKVersion;

	extern const std::string c_sBuildTag;

	extern const std::string c_sRemoteUrl;
	extern const std::string c_sBranch;
	extern const std::string c_sCommit;
	extern const bool c_bIsDirty;

	inline std::string GetDisplayVersion()
	{
		return c_sBuildTag;
	}

	inline std::string GetInternalVersion()
	{
		std::string version = c_sBranch + "@" + c_sCommit;
		if (c_bIsDirty)
		{
			version += "+dirty";
		}
		return version;
	}
};
