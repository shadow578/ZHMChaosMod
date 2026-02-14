#include "CompanionMod.h"

#include <Logging.h>

#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[CompanionModUtil] "

static const std::string c_sMetadataRepoId = "5a02ac25-a1c9-4a97-9e8a-7b07c84462cc";
static const std::string c_sMetadataMarker = "ZHMChaosMod_marker";

CompanionModUtil::SCompanionModMetadata CompanionModUtil::LoadCompanionModInfo(const bool p_bAllowLoad)
{
	static CompanionModUtil::SCompanionModMetadata g_Metadata = {};
	static bool g_bMetadataLoaded = false;

	if (g_bMetadataLoaded)
	{
		return g_Metadata;
	}

	g_Metadata.m_bPresent = false;
	g_Metadata.m_sVersion = "";
	g_Metadata.m_Version = { 0, 0, 0 };

	if (!p_bAllowLoad)
	{
		return g_Metadata;
	}

	g_bMetadataLoaded = true;

	Logger::Debug(TAG "Loading companion mod metadata...");

	auto& s_Repo = ZHMRepositoryHelper::GetInstance();
	s_Repo.Initialize();

	if (!s_Repo.IsLoaded())
	{
		Logger::Debug(TAG "Repository not loaded, deferring companion mod metadata load.");
		g_bMetadataLoaded = false; // allow retry later
		return g_Metadata;
	}

	const auto s_pMetadataObj = s_Repo.Get(c_sMetadataRepoId);
	if (!s_pMetadataObj)
	{
		Logger::Debug(TAG "Companion mod metadata not found in repository.");
		return g_Metadata;
	}

	// metadata must contain the marker key and value, otherwise ignore it
	const auto s_psMarkerOpt = s_pMetadataObj->Get<ZString>(c_sMetadataMarker);
	if (!s_psMarkerOpt || std::string(s_psMarkerOpt.value()->c_str()) != c_sMetadataMarker)
	{
		Logger::Debug(TAG "Companion mod metadata marker not found or invalid.");
		return g_Metadata;
	}
	g_Metadata.m_bPresent = true;
	
	const auto s_psVersionOpt = s_pMetadataObj->Get<ZString>("Version");
	if (s_psVersionOpt)
	{
		g_Metadata.m_sVersion = s_psVersionOpt.value()->c_str();

		// parse version to major, minor, patch ints
		const auto s_nDot = g_Metadata.m_sVersion.find('.');
		if (s_nDot != std::string::npos)
		{
			g_Metadata.m_Version.m_nMajor = std::stoi(g_Metadata.m_sVersion.substr(0, s_nDot));
			
			const auto s_nDot2 = g_Metadata.m_sVersion.find('.', s_nDot + 1);
			if (s_nDot2 != std::string::npos)
			{
				g_Metadata.m_Version.m_nMinor = std::stoi(g_Metadata.m_sVersion.substr(s_nDot + 1, s_nDot2 - s_nDot - 1));
				g_Metadata.m_Version.m_nPatch = std::stoi(g_Metadata.m_sVersion.substr(s_nDot2 + 1));
			}
			else
			{
				g_Metadata.m_Version.m_nMinor = std::stoi(g_Metadata.m_sVersion.substr(s_nDot + 1));
			}
		}
	}

	Logger::Debug(TAG "Detected companion mod version {} ({}.{}.{}).", 
		g_Metadata.m_sVersion,
		g_Metadata.m_Version.m_nMajor, g_Metadata.m_Version.m_nMinor, g_Metadata.m_Version.m_nPatch
	);
	return g_Metadata;
}

bool CompanionModUtil::IsCompanionModPresent()
{
	return LoadCompanionModInfo().m_bPresent;
}
