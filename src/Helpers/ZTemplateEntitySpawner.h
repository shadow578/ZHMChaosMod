#pragma once
#include "ZResourceProvider.h"

#include <string>
#include <vector>
#include <memory>

/**
 * Provides easy access to entity resources for spawning.
 * Automatically handles resource loading, unloading and entity spawning, despawning.
 * Usage:
 * auto s_pSpawner = ZTemplateEntitySpawner::Create<"[assembly:/example.entitytemplate].pc_entitytype">();
 */
class ZTemplateEntitySpawner : public ZResourceProvider
{
public:
	template <detail::StringLiteral ResPath>
	static std::unique_ptr<ZTemplateEntitySpawner> Create()
	{
		return std::make_unique<ZTemplateEntitySpawner>(ResPath.Value, ResId<ResPath>);
	}

	ZTemplateEntitySpawner(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId)
		: ZResourceProvider(p_sResourcePath, p_ResourceId)
	{

	}

	~ZTemplateEntitySpawner();

	/**
	 * Spawn entity from resource.
	 */
	ZEntityRef Spawn();
	
	/**
	 * Spawn entity from resource and query interface T.
	 * @template T entity interface to query
	 */
	template <typename T>
	TEntityRef<T> SpawnAs()
	{
		if (const auto s_EntityRef = Spawn())
		{
			return TEntityRef<T>(s_EntityRef);;
		}

		return {};
	}

	/**
	 * Despawn the entity.
	 * NOTE: there's no checks to ensure the entity was spawned by this spawner.
	 * @param p_rEntity Entity to despawn.
	 */
	void Despawn(ZEntityRef p_rEntity);

	/**
	 * Despawn all entities spawned by this spawner.
	 */
	void DespawnAll();

	/**
	 * Get details about the resource and its status as a string.
	 * Callable even if IsAvailable()==false.
	 */
	std::string ToString() const;

private:
	std::vector<ZEntityRef> m_aSpawnedEntities;
};
