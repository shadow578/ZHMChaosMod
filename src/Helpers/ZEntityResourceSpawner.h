#pragma once
#include <Glacier/ZResource.h>
#include <Glacier/ZEntity.h>
#include <Glacier/CompileReflection.h>
#include <Glacier/EntityFactory.h>

class ZEntityResourceSpawnerImpl
{
public:
	TResourcePtr<ZTemplateEntityFactory> GetResourcePointer() const;
	ZEntityRef Spawn() const;
	
	template <typename T>
	TEntityRef<T> SpawnAs() const
	{
		if (const auto s_EntityRef = Spawn())
		{
			return TEntityRef<T>(s_EntityRef);;
		}

		return {};
	}

	const ZRuntimeResourceID& GetResourceID() const
	{
		return m_ResourceID;
	}

	const std::string& GetResourcePath() const
	{
		return m_sResourcePath;
	}

	bool IsAvailable(const bool p_bAllowCheck = true);

	std::string ToString() const;

protected:
	ZEntityResourceSpawnerImpl(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId) :
		m_sResourcePath(p_sResourcePath),
		m_ResourceID(p_ResourceId)
	{
	}

private:
	const std::string m_sResourcePath;
	const ZRuntimeResourceID m_ResourceID;

	bool m_bAvailable = false;
	bool m_bAvailableChecked = false;
};

template <detail::StringLiteral ResPath>
class ZEntityResourceSpawner : public ZEntityResourceSpawnerImpl
{
public:
	ZEntityResourceSpawner() :
		ZEntityResourceSpawnerImpl(ResPath.Value, ResId<ResPath>) {}
};
