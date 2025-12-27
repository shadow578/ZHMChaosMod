#include "ZEntityResourceSpawner.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>
#include <Glacier/SExternalReferences.h>

#include <Globals.h>
#include <Functions.h>
#include <Logging.h>

#define TAG "[ZEntityResourceSpawner] "

TResourcePtr<ZTemplateEntityFactory> ZEntityResourceSpawnerImpl::GetResource() const
{
	const auto s_ResourceId = GetResourceID();

    TResourcePtr<ZTemplateEntityFactory> s_ResourcePtr;
    if (!Globals::ResourceManager->GetResourcePtr(s_ResourcePtr, s_ResourceId, 0) || !s_ResourcePtr)
    {
        Logger::Debug(TAG "resource '{}' ({:016X}) not loaded, attempting to load now", m_sResourcePath, m_ResourceID.GetID());

        if (!Globals::ResourceManager->LoadResource(s_ResourcePtr, s_ResourceId) || !s_ResourcePtr)
        {
            Logger::Debug(TAG "resource '{}' (:016X) failed to load", m_sResourcePath, m_ResourceID.GetID());
            return {};
        }

        while (!Globals::ResourceManager->DoneLoading())
        {
            Logger::Debug("Waiting for resources to load (left: {})!", Globals::ResourceManager->m_nNumProcessing);
            Globals::ResourceManager->Update(true);
        }
    }

    return s_ResourcePtr;
}

ZEntityRef ZEntityResourceSpawnerImpl::Spawn() const
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn, no scene loaded.");
        return {};
    }

	const auto s_PropFactory = GetResource();
    if (!s_PropFactory)
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
        s_PropFactory,
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
    return s_EntityRef;
}

bool ZEntityResourceSpawnerImpl::IsAvailable(const bool p_bAllowCheck)
{
    // not checked yet and not allowed to check
    if (!m_bAvailableChecked && !p_bAllowCheck)
    {
        return false;
    }

    // not checked, check now
    if (!m_bAvailableChecked)
    {
        m_bAvailable = GetResource();
		m_bAvailableChecked = true;
    }

    // use cached
    return m_bAvailable;
}

std::string ZEntityResourceSpawnerImpl::ToString() const
{
    return fmt::format("{} (ID={:016X}; Available={})",
        m_sResourcePath,
        m_ResourceID.GetID(),
        (m_bAvailableChecked ? (m_bAvailable ? "Y" : "N") : "?")
    );
}
