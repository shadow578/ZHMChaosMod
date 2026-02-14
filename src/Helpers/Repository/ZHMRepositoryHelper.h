#pragma once
#include <Glacier/ZPrimitives.h>
#include <Glacier/ZResource.h>
#include <Glacier/ZObject.h>
#include <Glacier/THashMap.h>

#include <map>
#include <string>
#include <optional>
#include <ranges>

#include "ZRepositoryEntryHelper.h"
#include "ZRepositoryEntryPatcher.h"

typedef THashMap<ZRepositoryID, ZDynamicObject, TDefaultHashMapPolicy<ZRepositoryID>> ZHMRepositoryData;

class ZHMRepositoryHelper
{
public:
	enum class EEntryType
	{
		NPC,
		Item,
		Weapon,
		Modifier,
		MapArea,
		Outfit,
		Setpiece,
		DifficultyParameter,
		AmmoConfig,
		MagazineConfig,
		AmmoBehaviour,
		MasteryItem,
		ScoreMultiplier,
		ItemBundle,
		ItemList,
		WeaponConfig,
		Unknown
	};

public:
	static ZHMRepositoryHelper& GetInstance()
	{
		static ZHMRepositoryHelper instance;
		return instance;
	}

	/**
	 * Initializes the repository helper.
	 * This should be called once after scene load. Repeated calls will be ignored until Reset() is called.
	 */
	void Initialize();

	/**
	 * Reset any cached repository data.
	 * This should be called on scene clear.
	 */
	void Reset();

	/**
	 * Is the repository data loaded and valid?
	 */
	bool IsLoaded() const
	{
		return m_RepositoryResource && m_RepositoryResource.GetResourceInfo().status == RESOURCE_STATUS_VALID;
	}

	/**
	 * Get a repository entry 
	 */
	ZDynamicObject& GetObj(const std::string& p_sId) const;
	ZDynamicObject& GetObj(const ZRepositoryID& p_RepoId) const;

	std::unique_ptr<ZRepositoryEntryHelper> Get(const std::string& p_sId) const;
	std::unique_ptr<ZRepositoryEntryHelper> Get(const ZRepositoryID& p_RepoId) const;

	std::unique_ptr<ZRepositoryEntryPatcher> Patch(const std::string& p_sId) const;
	std::unique_ptr<ZRepositoryEntryPatcher> Patch(const ZRepositoryID& p_RepoId) const;

	/**
	 * Enumerate all repository IDs of entries of the given type.
	 */
	auto GetEntryIdsByType(const EEntryType p_eType) const
	{
		return m_mRepositoryKeyToType
			| std::views::filter([p_eType](const auto& pair) { return pair.second == p_eType; })
			| std::views::transform([](const auto& pair) -> const ZRepositoryID& { return pair.first; });
	}

private:
	ZHMRepositoryHelper() {};
	ZHMRepositoryHelper(const ZHMRepositoryHelper&) = delete;
	ZHMRepositoryHelper& operator=(const ZHMRepositoryHelper&) = delete;

private:
	/**
	 * Pointer to the loaded repository data resource.
	 */
	TResourcePtr<ZHMRepositoryData> m_RepositoryResource = {};

	/**
	 * Maps each repository entries 'ID_' value to the corresponding
	 * repository ID (key to the repository THashMap)
	 */
	std::map<std::string, ZRepositoryID> m_mRepositoryIdToKey;

	/**
	 * Maps each repository ID (key to the repository THashMap) to the corresponding
	 * entry type.
	 */
	std::map<ZRepositoryID, EEntryType> m_mRepositoryKeyToType;

	static std::string GetIDFromEntry(const ZDynamicObject& p_Entry);
	static EEntryType GetTypeFromEntry(const ZDynamicObject& p_Entry);
};
