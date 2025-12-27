#include "EntityUtils.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>

#include "Globals.h"
#include "Logging.h"

#include <queue>

#define TAG "[EntityUtils] "

std::vector<ZEntityRef> Utils::EntityFinder::FindEntities(const SSearchParams& p_Params)
{
    std::vector<ZEntityRef> s_aFoundEntities;

    if (!Globals::Hitman5Module)
    {
        Logger::Error(TAG "Hitman5Module is null.");
        return s_aFoundEntities;
    }

    const auto s_pSceneCtx = Globals::Hitman5Module->m_pEntitySceneContext;
    if (!s_pSceneCtx)
    {
        Logger::Error(TAG "no active scene context.");
        return s_aFoundEntities;
    }

    std::queue<std::pair<ZEntityBlueprintFactoryBase*, ZEntityRef>> s_qNodeQueue;
    for (const auto& s_Brick : s_pSceneCtx->m_aLoadedBricks)
    {
        const auto s_Entity = s_Brick.m_EntityRef;
        if (!s_Entity)
        {
            continue;
        }

        const auto s_pBpFactory = s_Entity.GetBlueprintFactory();
        if (!s_pBpFactory)
        {
            continue;
        }

        s_qNodeQueue.emplace(s_pBpFactory, s_Entity);
    }

    while (!s_qNodeQueue.empty())
    {
        auto [s_pCurrentFactory, s_CurrentRoot] = s_qNodeQueue.front();
        s_qNodeQueue.pop();

        // go through each sub-entity
        const auto s_SubEntityCount = s_pCurrentFactory->GetSubEntitiesCount();
        for (int i = 0; i < s_SubEntityCount; ++i)
        {
            const ZEntityRef s_SubEntity = s_pCurrentFactory->GetSubEntity(s_CurrentRoot.m_pEntity, i);
            const auto s_SubEntityFactory = s_pCurrentFactory->GetSubEntityBlueprint(i);

            if (!s_SubEntity.GetEntity() || !s_SubEntity->GetType())
            {
                continue;
            }

            // if the sub-entity has a factory with more sub-entities, add it to the queue.
            if (s_SubEntityFactory && s_SubEntityFactory->GetSubEntitiesCount() > 0)
            {
                s_qNodeQueue.emplace(s_SubEntityFactory, s_SubEntity);
            }

            // check entity id
            if (p_Params.m_nEntityId != 0 &&
                s_SubEntity->GetType()->m_nEntityId == p_Params.m_nEntityId)
            {
                s_aFoundEntities.push_back(s_SubEntity);
            }

            // check entity name
            if (!p_Params.m_sEntityName.empty())
            {
                const auto s_sSubEntityName = GetEntityName(s_SubEntity, s_pCurrentFactory, i);
                if (s_sSubEntityName == p_Params.m_sEntityName)
                {
                    s_aFoundEntities.push_back(s_SubEntity);
                }
            }

            // check entity type
            if (!p_Params.m_sEntityType.empty())
            {
                const auto s_sSubEntityType = GetEntityTypeName(s_SubEntity);
                if (s_sSubEntityType == p_Params.m_sEntityType)
                {
                    s_aFoundEntities.push_back(s_SubEntity);
                }
            }
        }
    }

    return s_aFoundEntities;
}

std::string Utils::GetEntityName(const ZEntityRef& p_Entity, ZEntityBlueprintFactoryBase* p_pFactory, int p_nSubIndex)
{
    if (!p_Entity)
    {
        return "";
    }

    if (!p_pFactory)
    {
        p_pFactory = p_Entity.GetBlueprintFactory();
        if (!p_pFactory)
        {
            return "";
        }
    }

    if (p_nSubIndex < 0)
    {
        p_nSubIndex = p_pFactory->GetSubEntityIndex(p_Entity->GetType()->m_nEntityId);
        if (p_nSubIndex < 0)
        {
            return "";
        }
    }

    if (p_pFactory->IsTemplateEntityBlueprintFactory())
    {
        const auto s_pTemplateBpFactory = reinterpret_cast<ZTemplateEntityBlueprintFactory*>(p_pFactory);
        if (s_pTemplateBpFactory->m_pTemplateEntityBlueprint)
        {
            return s_pTemplateBpFactory->m_pTemplateEntityBlueprint->subEntities[p_nSubIndex].entityName.c_str();
        }
    }

    if (p_pFactory->IsAspectEntityBlueprintFactory())
    {
        const auto s_pAspectBpFactory = reinterpret_cast<ZAspectEntityBlueprintFactory*>(p_pFactory);
        const auto s_SubentityEntry = s_pAspectBpFactory->m_aSubEntitiesLookUp[p_nSubIndex];
        const auto s_nAspectIndex = s_SubentityEntry.m_nAspectIdx;
        const auto s_nSubentityIndex = s_SubentityEntry.m_nSubentityIdx;

        const auto s_pTemplateBpFactory = reinterpret_cast<ZTemplateEntityBlueprintFactory*>(
            s_pAspectBpFactory->m_aBlueprintFactories[s_nAspectIndex]);
        if (s_pTemplateBpFactory->m_pTemplateEntityBlueprint)
        {
            return s_pTemplateBpFactory->m_pTemplateEntityBlueprint->subEntities[s_nSubentityIndex].entityName.c_str();
        }
    }

    return "";
}

std::string Utils::GetEntityTypeName(const ZEntityRef& p_Entity)
{
    if (!p_Entity)
    {
        return "";
    }

    // ´FIXME ugly unpacking
    if (const auto s_pEntity = p_Entity.GetEntity())
    {
        if (const auto s_pType = s_pEntity->GetType(); s_pType->m_pInterfaces)
        {
            if (const auto s_pTypeId = (*s_pType->m_pInterfaces)[0].m_pTypeId)
            {
                if (const auto s_pTypeInfo = s_pTypeId->typeInfo())
                {
                    if (const auto s_pName = s_pTypeInfo->m_pTypeName)
                    {
                        return s_pName;
                    }
                }
            }
        }
    }

    return "";
}