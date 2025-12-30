#pragma once
#include <Glacier/ZResource.h>
#include <Glacier/ZEntity.h>
#include <Glacier/CompileReflection.h>
#include <Glacier/EntityFactory.h>

#include <string>
#include <vector>
#include <memory>

#include "Utils.h"

class ZResourceProviderSession
{
public:
	ZResourceProviderSession(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId);
	~ZResourceProviderSession();

	const ZRuntimeResourceID& GetResourceID() const
	{
		return m_ResourceID;
	}

	const std::string& GetResourcePath() const
	{
		return m_sResourcePath;
	}

	const ZResourcePtr& GetResource() const
	{
		return m_ResourcePtr;
	}

	template <typename T>
	const TResourcePtr<T>& GetResourceAs() const
	{
		return static_cast<const TResourcePtr<T>&>(m_ResourcePtr);
	}

	bool IsAvailable() const;

	std::string ToString() const;

private:
	const std::string m_sResourcePath;
	const ZRuntimeResourceID m_ResourceID;
	ZResourcePtr m_ResourcePtr;

	bool LoadResource(ZResourcePtr& p_ResourcePtr) const;
};

template <detail::StringLiteral ResPath>
class ZResourceProvider
{
public:
	std::unique_ptr<ZResourceProviderSession> CreateSession() const
	{
		return std::make_unique<ZResourceProviderSession>(ResPath.Value, ResId<ResPath>);
	}
};
