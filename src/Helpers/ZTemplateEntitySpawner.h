#pragma once
#include "ZResourceProvider.h"

#include <string>
#include <vector>
#include <memory>

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

	ZEntityRef Spawn();
	
	template <typename T>
	TEntityRef<T> SpawnAs()
	{
		if (const auto s_EntityRef = Spawn())
		{
			return TEntityRef<T>(s_EntityRef);;
		}

		return {};
	}

	void Despawn(ZEntityRef p_rEntity);
	void DespawnAll();

	std::string ToString() const;

private:
	std::vector<ZEntityRef> m_aSpawnedEntities;
};
