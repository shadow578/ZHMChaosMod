#include "ZTemplateEntitySpawner.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>
#include <Glacier/SExternalReferences.h>

#include <Globals.h>
#include <Functions.h>
#include <Logging.h>

#define TAG "[ZTemplateEntitySpawner] "

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

ZTemplateEntitySpawnerSession::ZTemplateEntitySpawnerSession(const std::string p_sResourcePath, const ZRuntimeResourceID p_ResourceId) :
    m_sResourcePath(p_sResourcePath),
    m_ResourceID(p_ResourceId)
{
    if (!LoadResource(m_ResourcePtr))
    {
		Logger::Warn(TAG "Failed to load resource '{}' ({:016X}).", m_sResourcePath, m_ResourceID.GetID());
    }
}

ZTemplateEntitySpawnerSession::~ZTemplateEntitySpawnerSession()
{
    // despawn entities
    // since we (potentially) uninstall the resource of the entities when m_ResourcePtr is released,
	// we need to make sure to delete the entities first.
	// otherwise, the entities may linger around with invalid resource pointers, leading to crashes shortly after.
    // alternatively, one could also just leave the resource loaded, but this is cleaner.
    for (auto& s_Entity : m_aSpawnedEntities)
    {
        if (s_Entity)
        {
		    Functions::ZEntityManager_DeleteEntity->Call(Globals::EntityManager, s_Entity, {});
        }
    }

	// m_ResourcePtr will be automatically released
}

bool ZTemplateEntitySpawnerSession::LoadResource(TResourcePtr<ZTemplateEntityFactory>& p_ResourcePtr) const
{
    const auto& s_ResourceId = GetResourceID();

    if (!Globals::ResourceManager->GetResourcePtr(p_ResourcePtr, s_ResourceId, 0) || !p_ResourcePtr)
    {
        Logger::Debug(TAG "resource '{}' ({:016X}) not loaded, attempting to load now.", m_sResourcePath, m_ResourceID.GetID());

        if (!Globals::ResourceManager->LoadResource(p_ResourcePtr, s_ResourceId) || !p_ResourcePtr)
        {
            Logger::Debug(TAG "resource '{}' (:016X) failed to load", m_sResourcePath, m_ResourceID.GetID());
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

    Logger::Debug(TAG "resource '{}' ({:016X}) loaded; status={}, refCount={}.",
        m_sResourcePath, 
        m_ResourceID.GetID(),
        ResourceStatusToString(s_Info.status),
		s_Info.refCount
    );
	return !!p_ResourcePtr;
}

ZEntityRef ZTemplateEntitySpawnerSession::Spawn()
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn, no scene loaded.");
        return {};
    }

    if (!m_ResourcePtr)
    {
        Logger::Debug(TAG "Cannot spawn entity from resource '{}', resource not loaded.", m_sResourcePath);
        return {};
    }

	ZEntityRef s_EntityRef;
    SExternalReferences s_ExternalRefs;
    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        s_EntityRef,
        "",
        m_ResourcePtr,
        s_Scene.m_ref,
        s_ExternalRefs,
        -1
    );

    if (!s_EntityRef)
    {
        Logger::Debug(TAG "Spawning entity from resource '{}' failed.", m_sResourcePath);
        return {};
    }

    Logger::Debug(TAG "Spawned entity from resource '{}'.", m_sResourcePath);

	m_aSpawnedEntities.push_back(s_EntityRef);
    return s_EntityRef;
}

bool ZTemplateEntitySpawnerSession::IsAvailable() const
{
    if (!m_ResourcePtr)
    {
        return false;
	}

    const auto& s_Info = m_ResourcePtr.GetResourceInfo();
    return s_Info.refCount > 0 && s_Info.status == RESOURCE_STATUS_VALID;
}

std::string ZTemplateEntitySpawnerSession::ToString() const
{
    return fmt::format("{} (RID={:016X}; ResIndex={}, ResStatus={}, ResRefCount={}; Active Entities={})",
        GetResourcePath(),
        GetResourceID().GetID(),
		(IsAvailable() ? std::to_string(m_ResourcePtr.m_nResourceIndex.val) : "N/A"),
        (IsAvailable() ? ResourceStatusToString(m_ResourcePtr.GetResourceInfo().status) : "N/A"),
        (IsAvailable() ? std::to_string(m_ResourcePtr.GetResourceInfo().refCount) : "N/A"),
		m_aSpawnedEntities.size()
    );
}
