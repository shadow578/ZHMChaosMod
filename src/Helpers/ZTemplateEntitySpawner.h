#pragma once
#include "ZResourceProvider.h"

#include <string>
#include <vector>
#include <memory>

class ZTemplateEntitySpawnerSession : public ZResourceProviderSession
{
public:
	ZTemplateEntitySpawnerSession(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId)
		: ZResourceProviderSession(p_sResourcePath, p_ResourceId)
	{

	}

	~ZTemplateEntitySpawnerSession();

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

template <detail::StringLiteral ResPath>
class ZTemplateEntitySpawner
{
public:
	std::unique_ptr<ZTemplateEntitySpawnerSession> CreateSession() const
	{
		return std::make_unique<ZTemplateEntitySpawnerSession>(ResPath.Value, ResId<ResPath>);
	}
};
