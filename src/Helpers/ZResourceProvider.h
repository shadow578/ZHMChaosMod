#pragma once
#include <Glacier/ZResource.h>
#include <Glacier/ZEntity.h>
#include <Glacier/CompileReflection.h>
#include <Glacier/EntityFactory.h>

#include <string>
#include <vector>
#include <memory>

#include "Utils.h"

class ZResourceProvider
{
public:
	template <detail::StringLiteral ResPath>
	static std::unique_ptr<ZResourceProvider> Create()
	{
		return std::make_unique<ZResourceProvider>(ResPath.Value, ResId<ResPath>);
	}

	ZResourceProvider(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId);
	~ZResourceProvider();

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
