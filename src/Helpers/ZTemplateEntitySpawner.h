#pragma once
#include <Glacier/ZResource.h>
#include <Glacier/ZEntity.h>
#include <Glacier/CompileReflection.h>
#include <Glacier/EntityFactory.h>

#include <string>
#include <vector>
#include <memory>

class ZTemplateEntitySpawnerSession
{
public:
	ZTemplateEntitySpawnerSession(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId);
	~ZTemplateEntitySpawnerSession();

	const ZRuntimeResourceID& GetResourceID() const
	{
		return m_ResourceID;
	}

	const std::string& GetResourcePath() const
	{
		return m_sResourcePath;
	}

	bool IsAvailable() const;

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

	std::string ToString() const;

private:
	const std::string m_sResourcePath;
	const ZRuntimeResourceID m_ResourceID;
	TResourcePtr<ZTemplateEntityFactory> m_ResourcePtr;
	std::vector<ZEntityRef> m_aSpawnedEntities;

	bool LoadResource(TResourcePtr<ZTemplateEntityFactory>& p_ResourcePtr) const;
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
