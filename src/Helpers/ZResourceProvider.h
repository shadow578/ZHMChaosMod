#pragma once
#include <Glacier/ZResource.h>
#include <Glacier/ZEntity.h>
#include <Glacier/CompileReflection.h>
#include <Glacier/EntityFactory.h>

#include <string>
#include <vector>
#include <memory>

#include "Utils.h"

/**
 * Provides easy access to resources at runtime. 
 * Automatically handles loading, unloading and lifetimes.
 * Usage:
 * auto s_pResource = ZResourceProvider::Create<"[assembly:/example.entitytemplate].pc_entitytype">();
 */
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

	/**
	 * Get resource id entered in constructor.
	 */
	const ZRuntimeResourceID& GetResourceID() const
	{
		return m_ResourceID;
	}

	/**
	 * Get resource path string entered in constructor.
	 */
	const std::string& GetResourcePath() const
	{
		return m_sResourcePath;
	}

	/**
	 * Get the resource pointer to the resource, if IsAvailable()==true.
	 */
	const ZResourcePtr& GetResource() const
	{
		return m_ResourcePtr;
	}

	/**
	 * Get the resource pointer as a specific pointer type, if IsAvailable()==true.
	 */
	template <typename T>
	const TResourcePtr<T>& GetResourceAs() const
	{
		return static_cast<const TResourcePtr<T>&>(m_ResourcePtr);
	}

	/**
	 * Is this resource available for use?
	 */
	bool IsAvailable() const;

	/**
	 * Get details about the resource and its status as a string.
	 * Callable even if IsAvailable()==false.
	 */
	std::string ToString() const;

private:
	const std::string m_sResourcePath;
	const ZRuntimeResourceID m_ResourceID;
	ZResourcePtr m_ResourcePtr;

	bool LoadResource(ZResourcePtr& p_ResourcePtr) const;
};
