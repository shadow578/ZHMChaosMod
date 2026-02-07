#include "EntityUtils.h"

#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>

#include "Globals.h"
#include "Logging.h"

#include <queue>

#define TAG "[EntityUtils] "

std::vector<ZEntityRef> Utils::ZEntityFinder::Find(const size_t p_nMaxResults) const
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

    std::queue<std::pair<ZEntityBlueprintFactoryBase *, ZEntityRef>> s_qNodeQueue;
    for (const auto &s_Brick : s_pSceneCtx->m_aLoadedBricks)
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
            const ZEntityRef s_SubEntity = s_pCurrentFactory->GetSubEntity(s_CurrentRoot.m_pObj, i);
            const auto s_pSubEntityFactory = s_pCurrentFactory->GetSubEntityBlueprint(i);

            if (!s_SubEntity.GetEntity() || !s_SubEntity->GetType())
            {
                continue;
            }

            // if the sub-entity has a factory with more sub-entities, add it to the queue.
            if (s_pSubEntityFactory && s_pSubEntityFactory->GetSubEntitiesCount() > 0)
            {
                s_qNodeQueue.emplace(s_pSubEntityFactory, s_SubEntity);
            }

            // add if matches
            if (Evaluate(s_SubEntity, s_pCurrentFactory, s_pSubEntityFactory, i))
            {
                s_aFoundEntities.push_back(s_SubEntity);

                // check max results limit
                if (p_nMaxResults != 0 && s_aFoundEntities.size() >= p_nMaxResults)
                {
                    goto after_outer_loop;
                }
            }
        }
    }

after_outer_loop:
    return s_aFoundEntities;
}

bool Utils::ZEntityFinder::Evaluate(const ZEntityRef &p_rEntity, ZEntityBlueprintFactoryBase *p_pParentFactory, ZEntityBlueprintFactoryBase *p_pSubFactory, int p_nSubIndex) const
{
    // check entity id
    if (m_nEntityID.has_value())
    {
        if (p_rEntity->GetType()->m_nEntityID != m_nEntityID.value())
        {
            return false;
        }
    }

    // check entity name
    if (m_sEntityName.has_value())
    {
        const auto s_sSubEntityName = GetEntityName(p_rEntity, p_pParentFactory, p_nSubIndex);
        if (s_sSubEntityName != m_sEntityName.value())
        {
            return false;
        }
    }

    // check entity type
    if (m_sEntityType.has_value())
    {
        const auto s_sSubEntityType = GetEntityTypeName(p_rEntity);
        if (s_sSubEntityType != m_sEntityType.value())
        {
            return false;
        }
    }

    // check blueprint resource
    if (m_ridBlueprint.has_value())
    {
        if (!p_pSubFactory || p_pSubFactory->m_ridResource != m_ridBlueprint.value())
        {
            return false;
        }
    }

    return true;
}

std::string Utils::GetEntityName(const ZEntityRef &p_Entity, ZEntityBlueprintFactoryBase *p_pFactory, int p_nSubIndex)
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
        p_nSubIndex = p_pFactory->GetSubEntityIndex(p_Entity->GetType()->m_nEntityID);
        if (p_nSubIndex < 0)
        {
            return "";
        }
    }

    if (p_pFactory->IsTemplateEntityBlueprintFactory())
    {
        const auto s_pTemplateBpFactory = reinterpret_cast<ZTemplateEntityBlueprintFactory *>(p_pFactory);
        if (s_pTemplateBpFactory->m_pTemplateEntityBlueprint)
        {
            return s_pTemplateBpFactory->m_pTemplateEntityBlueprint->subEntities[p_nSubIndex].entityName.c_str();
        }
    }

    if (p_pFactory->IsAspectEntityBlueprintFactory())
    {
        const auto s_pAspectBpFactory = reinterpret_cast<ZAspectEntityBlueprintFactory *>(p_pFactory);
        const auto s_SubentityEntry = s_pAspectBpFactory->m_aSubEntitiesLookUp[p_nSubIndex];
        const auto s_nAspectIndex = s_SubentityEntry.m_nAspectIdx;
        const auto s_nSubentityIndex = s_SubentityEntry.m_nSubentityIdx;

        const auto s_pTemplateBpFactory = reinterpret_cast<ZTemplateEntityBlueprintFactory *>(
            s_pAspectBpFactory->m_aBlueprintFactories[s_nAspectIndex]);
        if (s_pTemplateBpFactory->m_pTemplateEntityBlueprint)
        {
            return s_pTemplateBpFactory->m_pTemplateEntityBlueprint->subEntities[s_nSubentityIndex].entityName.c_str();
        }
    }

    return "";
}

std::string Utils::GetEntityTypeName(const ZEntityRef &p_Entity)
{
    if (!p_Entity)
    {
        return "";
    }

    // FIXME ugly unpacking
    if (const auto s_pEntity = p_Entity.GetEntity())
    {
        if (const auto s_pType = s_pEntity->GetType(); s_pType->m_pInterfaceData)
        {
            if (const auto s_pTypeId = (*s_pType->m_pInterfaceData)[0].m_Type)
            {
                if (const auto s_pTypeInfo = s_pTypeId->GetTypeInfo())
                {
                    if (const auto s_pName = s_pTypeInfo->pszTypeName)
                    {
                        return s_pName;
                    }
                }
            }
        }
    }

    return "";
}

ZEntityBlueprintFactoryBase *Utils::GetEntityBlueprintFactoryFor(ZEntityRef p_rEntity)
{
    // has BP factory?
    if (auto *s_pBpFactory = p_rEntity.GetBlueprintFactory())
    {
        return s_pBpFactory;
    }

    // try get sub-blueprint from owner
    if (auto *s_pParentBPFactory = GetEntityBlueprintFactoryFor(p_rEntity.GetOwningEntity()))
    {
        const auto s_nEntityId = p_rEntity.GetEntity()->GetType()->m_nEntityID;
        if (const auto s_nSubIndex = s_pParentBPFactory->GetSubEntityIndex(s_nEntityId); s_nSubIndex != -1)
        {
            auto *s_pBPFactory = s_pParentBPFactory->GetSubEntityBlueprint(s_nSubIndex);

            // if aspect, get template sub-blueprint
            // TODO untested, does this work?!
            if (s_pBPFactory->IsAspectEntityBlueprintFactory())
            {
                const auto *s_pAspectBPFactory = reinterpret_cast<const ZAspectEntityBlueprintFactory *>(s_pBPFactory);

                const auto s_AspectSubIndex = s_pAspectBPFactory->m_aSubEntitiesLookUp[s_nSubIndex];

                s_pBPFactory = reinterpret_cast<ZEntityBlueprintFactoryBase *>(s_pAspectBPFactory->m_aBlueprintFactories[s_AspectSubIndex.m_nSubentityIdx]);
            }

            return s_pBPFactory;
        }
    }

    // no luck :(
    return nullptr;
}
