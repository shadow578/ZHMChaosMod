#include "ZTemplateEntitySpawner.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>
#include <Glacier/SExternalReferences.h>

#include <Globals.h>
#include <Functions.h>
#include <Logging.h>

#define TAG "[ZTemplateEntitySpawner] "

ZTemplateEntitySpawner::~ZTemplateEntitySpawner()
{
    // since we (potentially) uninstall the resource of the entities when m_ResourcePtr is released,
    // we need to make sure to delete the entities first.
    // otherwise, the entities may linger around with invalid resource pointers, leading to crashes shortly after.
    // alternatively, one could also just leave the resource loaded, but this is cleaner.
    DespawnAll();

    // free resource
    ZResourceProvider::~ZResourceProvider();
}

ZEntityRef ZTemplateEntitySpawner::Spawn()
{
    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Cannot spawn, no scene loaded.");
        return {};
    }

    auto &s_ResourcePtr = GetResourceAs<ZTemplateEntityFactory>();
    if (!s_ResourcePtr)
    {
        Logger::Debug(TAG "Cannot spawn entity from resource '{}', resource not loaded.", GetResourcePath());
        return {};
    }

    ZEntityRef s_EntityRef;
    SExternalReferences s_ExternalRefs;
    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        s_EntityRef,
        "",
        s_ResourcePtr,
        s_Scene.m_entityRef,
        s_ExternalRefs,
        -1);

    if (!s_EntityRef)
    {
        Logger::Debug(TAG "Spawning entity from resource '{}' failed.", GetResourcePath());
        return {};
    }

    Logger::Debug(TAG "Spawned entity from resource '{}'.", GetResourcePath());

    m_aSpawnedEntities.push_back(s_EntityRef);
    return s_EntityRef;
}

void ZTemplateEntitySpawner::Despawn(ZEntityRef p_rEntity)
{
    if (!p_rEntity)
    {
        return;
    }

    m_aSpawnedEntities.erase(
        std::remove_if(
            m_aSpawnedEntities.begin(),
            m_aSpawnedEntities.end(),
            [&](const ZEntityRef &p_Ref)
            {
                return p_Ref == p_rEntity;
            }),
        m_aSpawnedEntities.end());

    Functions::ZEntityManager_DeleteEntity->Call(Globals::EntityManager, p_rEntity, {});
}

void ZTemplateEntitySpawner::DespawnAll()
{
    for (auto &s_Entity : m_aSpawnedEntities)
    {
        // note: not using Despawn() here to avoid modifying m_aSpawnedEntities while iterating over it
        if (s_Entity)
        {
            Functions::ZEntityManager_DeleteEntity->Call(Globals::EntityManager, s_Entity, {});
        }
    }

    m_aSpawnedEntities.clear();
}

std::string ZTemplateEntitySpawner::ToString() const
{
    return fmt::format("{}; Active Entities: {}",
                       ZResourceProvider::ToString(),
                       m_aSpawnedEntities.size());
}
