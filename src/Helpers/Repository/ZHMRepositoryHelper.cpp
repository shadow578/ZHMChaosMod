#include "ZHMRepositoryHelper.h"

#include <Logging.h>

#define TAG "[ZHMRepositoryHelper] "

void ZHMRepositoryHelper::Initialize()
{
	if (IsLoaded())
	{
		return;
	}

	// load repository
	const auto s_ridRepository = ResId<"[assembly:/repository/pro.repo].pc_repo">;
	Globals::ResourceManager->GetResourcePtr(m_RepositoryResource, s_ridRepository, 0);
	if (!IsLoaded())
	{
		Logger::Debug(TAG "Failed to load repository resource");
		return;
	}

	// populate caches
#if _DEBUG
	std::map<EEntryType, size_t> s_TypeCounts;
#endif

	const auto s_pmRepositoryData = m_RepositoryResource.GetResource();
	for (const auto& [s_RepositoryId, s_Entry] : *s_pmRepositoryData)
	{
		// ID_ to RepositoryID
		const auto s_sId = GetIDFromEntry(s_Entry);
		if (!s_sId.empty())
		{
			m_mRepositoryIdToKey[s_sId] = s_RepositoryId;
		}

		// RepositoryID to entry type
		const auto s_eType = GetTypeFromEntry(s_Entry);
		m_mRepositoryKeyToType[s_RepositoryId] = s_eType;

#if _DEBUG
		s_TypeCounts[s_eType]++;
#endif
	}

	Logger::Debug(TAG "Repository loaded. Total {} entries, {} cached IDs, {} cached types.",
		s_pmRepositoryData->size(),
		m_mRepositoryIdToKey.size(),
		m_mRepositoryKeyToType.size()
	);

#if _DEBUG
	for (const auto& [eType, count] : s_TypeCounts)
	{
		Logger::Debug(TAG " - Entries of Type {}: {}", static_cast<int>(eType), count);
	}
#endif
}

void ZHMRepositoryHelper::Reset()
{
	m_RepositoryResource = {};
	m_mRepositoryIdToKey.clear();
	m_mRepositoryKeyToType.clear();
}

ZDynamicObject& ZHMRepositoryHelper::GetObj(const std::string& p_sId) const
{
	// lookup cache first
	const auto s_It = m_mRepositoryIdToKey.find(p_sId);
	if (s_It != m_mRepositoryIdToKey.end())
	{
		return GetObj(s_It->second);
	}

	// assume p_sId is actually a repository ID string and try to parse it
	const ZRepositoryID s_RepoId(p_sId);
	if (s_RepoId == ZRepositoryID())
	{
		Logger::Debug(TAG "Invalid repository ID string: %s", p_sId.c_str());
		static ZDynamicObject s_EmptyObj;
		return s_EmptyObj;
	}

	return GetObj(s_RepoId);
}

ZDynamicObject& ZHMRepositoryHelper::GetObj(const ZRepositoryID& p_RepoId) const
{
	const auto s_pmRepositoryData = m_RepositoryResource.GetResource();

	const auto s_It = s_pmRepositoryData->find(p_RepoId);
	if (s_It != s_pmRepositoryData->end())
	{
		return s_It->second;
	}

	static ZDynamicObject s_EmptyObj;
	return s_EmptyObj;
}

std::unique_ptr<ZRepositoryEntryAccessor> ZHMRepositoryHelper::Get(const std::string& p_sId) const
{
	return ZRepositoryEntryAccessor::FromEntry(GetObj(p_sId));
}

std::unique_ptr<ZRepositoryEntryAccessor> ZHMRepositoryHelper::Get(const ZRepositoryID& p_RepoId) const
{
	return ZRepositoryEntryAccessor::FromEntry(GetObj(p_RepoId));
}

std::unique_ptr<ZRepositoryEntryPatcher> ZHMRepositoryHelper::Patch(const std::string& p_sId) const
{
	return std::make_unique<ZRepositoryEntryPatcher>(Get(p_sId));
}

std::unique_ptr<ZRepositoryEntryPatcher> ZHMRepositoryHelper::Patch(const ZRepositoryID& p_RepoId) const
{
	return std::make_unique<ZRepositoryEntryPatcher>(Get(p_RepoId));
}

std::string ZHMRepositoryHelper::GetIDFromEntry(const ZDynamicObject& p_Entry)
{
	const auto s_pEntry = ZRepositoryEntryAccessor::FromEntry(p_Entry);
	const auto s_sIdOpt = s_pEntry->Get<ZString>("ID_");

	if (!s_sIdOpt.has_value())
	{
		return "";
	}

	return s_sIdOpt.value()->c_str();
}

ZHMRepositoryHelper::EEntryType ZHMRepositoryHelper::GetTypeFromEntry(const ZDynamicObject& p_Entry)
{
	const auto s_pEntry = ZRepositoryEntryAccessor::FromEntry(p_Entry);

	// detection based on GlacierKit
	// https://github.com/atampy25/glacierkit/blob/4a503547299b7f3b3395324380d836674d651b49/src-tauri/src/event_handling/repository_patch.rs#L24

	if (s_pEntry->HasKey("ItemType"))
	{
		return s_pEntry->HasKey("PrimaryConfiguration") ? EEntryType::Weapon : EEntryType::Item;
	}
	else if (s_pEntry->HasKey("OutfitVariationIndex"))
	{
		return EEntryType::NPC;
	}
	else if (s_pEntry->HasKey("IsHitmanSuit"))
	{
		return EEntryType::Outfit;
	}
	else if (s_pEntry->HasKey("PersistentBoolId"))
	{
		return EEntryType::MapArea;
	}
	else if (s_pEntry->HasKey("OnlineTraits") && s_pEntry->HasKey("Features_")) // TODO: len checks omitted
	{
		return EEntryType::Setpiece;
	}
	else if (s_pEntry->HasKey("ModifierType"))
	{
		return EEntryType::Modifier;
	}
	else if (s_pEntry->HasKey("Parameter"))
	{
		return EEntryType::DifficultyParameter;
	}
	else if (s_pEntry->HasKey("AmmoConfig"))
	{
		return EEntryType::MagazineConfig;
	}
	else if (s_pEntry->HasKey("AmmoImpactEffect"))
	{
		return EEntryType::AmmoConfig;
	}
	else if (s_pEntry->HasKey("PenetratesEnvironment") || s_pEntry->HasKey("DeathContext"))
	{
		return EEntryType::AmmoBehaviour;
	}
	else if (s_pEntry->HasKey("Name") && s_pEntry->HasKey("Description") && s_pEntry->HasKey("Image"))
	{
		return EEntryType::MasteryItem;
	}
	else if (s_pEntry->HasKey("Piercing"))
	{
		return EEntryType::WeaponConfig;
	}
	else if (s_pEntry->HasKey("Multiplier"))
	{
		return EEntryType::ScoreMultiplier;
	}
	else if (s_pEntry->HasKey("CommonName") && s_pEntry->HasKey("Items"))
	{
		return EEntryType::ItemBundle;
	}
	else if (s_pEntry->HasKey("Guids"))
	{
		return EEntryType::ItemList;
	}

	return EEntryType::Unknown;
}
