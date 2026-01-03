#include "ZResourceProvider.h"

#include <Globals.h>
#include <Functions.h>
#include <Logging.h>

#define TAG "[ZResourceProvider] "

static std::string ResourceStatusToString(const EResourceStatus p_eStatus)
{
	switch (p_eStatus)
	{
	case RESOURCE_STATUS_UNKNOWN:    return "UNKNOWN";
	case RESOURCE_STATUS_LOADING:    return "LOADING";
	case RESOURCE_STATUS_INSTALLING: return "INSTALLING";
	case RESOURCE_STATUS_FAILED:     return "FAILED";
	case RESOURCE_STATUS_VALID:      return "VALID";
	default:                         return "N/A";
	}
}

ZResourceProvider::ZResourceProvider(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId) :
	m_sResourcePath(p_sResourcePath),
	m_ResourceID(p_ResourceId)
{
	if (!LoadResource(m_ResourcePtr))
	{
		Logger::Warn(TAG "Failed to load resource '{}' ({:016X}).", m_sResourcePath, m_ResourceID.GetID());
	}
}

ZResourceProvider::~ZResourceProvider()
{
	// m_ResourcePtr will be automatically released
}

bool ZResourceProvider::IsAvailable() const
{
	if (!m_ResourcePtr)
	{
		return false;
	}

	const auto& s_Info = m_ResourcePtr.GetResourceInfo();
	return s_Info.refCount > 0 && s_Info.status == RESOURCE_STATUS_VALID;
}

std::string ZResourceProvider::ToString() const
{
	return fmt::format("{} (RID={:016X}; ResIndex={}, ResStatus={}, ResRefCount={})",
		GetResourcePath(),
		GetResourceID().GetID(),
		(IsAvailable() ? std::to_string(m_ResourcePtr.m_nResourceIndex.val) : "N/A"),
		(IsAvailable() ? ResourceStatusToString(m_ResourcePtr.GetResourceInfo().status) : "N/A"),
		(IsAvailable() ? std::to_string(m_ResourcePtr.GetResourceInfo().refCount) : "N/A")
	);
}

bool ZResourceProvider::LoadResource(ZResourcePtr& p_ResourcePtr) const
{
	const auto& s_ResourceId = GetResourceID();

	if (!Globals::ResourceManager->GetResourcePtr(p_ResourcePtr, s_ResourceId, 0) || !p_ResourcePtr)
	{
		Logger::Debug(TAG "Resource '{}' ({:016X}) not loaded, attempting to load now.", m_sResourcePath, m_ResourceID.GetID());

		if (!Globals::ResourceManager->LoadResource(p_ResourcePtr, s_ResourceId) || !p_ResourcePtr)
		{
			Logger::Debug(TAG "Resource '{}' (:016X) failed to load", m_sResourcePath, m_ResourceID.GetID());
			p_ResourcePtr = {};
			return false;
		}

		while (!Globals::ResourceManager->DoneLoading())
		{
			Logger::Debug("Waiting for resources to load (left: {})!", Globals::ResourceManager->m_nNumProcessing);
			Globals::ResourceManager->Update(true);
		}
	}

	const auto& s_Info = p_ResourcePtr.GetResourceInfo();
	Logger::Debug(TAG "Resource '{}' ({:016X}) loaded; status={}, refCount={}.",
		m_sResourcePath,
		m_ResourceID.GetID(),
		ResourceStatusToString(s_Info.status),
		s_Info.refCount
	);
	return !!p_ResourcePtr;
}