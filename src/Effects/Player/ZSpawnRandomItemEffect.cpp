#include "ZSpawnRandomItemEffect.h"

#include <Glacier/ZContentKitManager.h>
#include <Glacier/ZInventory.h>
#include <Glacier/ZModule.h>
#include <Glacier/ZHitman5.h>
#include <Glacier/ZItem.h>
#include <Glacier/SExternalReferences.h>

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/Math.h"

#define TAG "[ZSpawnRandomItemEffect] "

static std::vector<std::pair<ZRepositoryID, std::string>> g_aRepositoryProps;

void ZSpawnRandomItemEffect::LoadResources()
{
    m_bIsAvailable = LoadRepositoryProps();
}

void ZSpawnRandomItemEffect::OnClearScene()
{
    g_aRepositoryProps.clear();
}

void ZSpawnRandomItemEffect::Start()
{
    if (const auto s_Player = SDK()->GetLocalPlayer())
    {
        if (const auto s_SpatialEntity = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
        {
            auto s_WM = s_SpatialEntity->GetObjectToWorldMatrix();

            // spawn a bit above and in front of the player
            const auto s_Forward = (-s_WM.Backward).Normalized();
            const auto s_Right = s_WM.Right.Normalized();
            const auto s_Up = s_WM.Up.Normalized();
            s_WM.Trans += s_Forward * 1.2f + s_Up * 1.0f;

            for (int i = 0; i < m_pCount; i++)
            {
                // vary spawn position a bit
                s_WM.Trans += s_Right * Math::GetRandomNumber(-0.2f, 0.2f) + s_Forward * Math::GetRandomNumber(-0.2f, 0.2f) + s_Up * Math::GetRandomNumber(-0.2f, 0.2f);

                const auto s_Selection = Math::SelectRandomElement(g_aRepositoryProps);

                Logger::Debug(TAG "spawning item {}", s_Selection.second);
                if (!SpawnRepositoryPropAt(s_Selection.first, s_WM))
                {
                    Logger::Debug(TAG "failed to spawn repository prop");
                }
            }
        }
    }
}

void ZSpawnRandomItemEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Repository Props Loaded:", g_aRepositoryProps.size()).c_str());
}

bool ZSpawnRandomItemEffect::SpawnRepositoryPropAt(const ZRepositoryID &p_RepositoryId, const SMatrix s_Transform)
{
    const auto s_pScene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_pScene)
    {
        Logger::Debug(TAG "no scene loaded");
        return false;
    }

    const auto s_SpawnerId = ResId<"[modules:/zitemspawner.class].pc_entitytype">;
    const auto s_KeyEntityId = ResId<"[modules:/zitemrepositorykeyentity.class].pc_entitytype">;

    TResourcePtr<ZTemplateEntityFactory> s_SpawnerResource, s_KeyEntityResource;
    if (!Globals::ResourceManager->GetResourcePtr(s_SpawnerResource, s_SpawnerId, 0) ||
        !Globals::ResourceManager->GetResourcePtr(s_KeyEntityResource, s_KeyEntityId, 0))
    {
        Logger::Debug(TAG "failed to get resource pointers");
        return false;
    }

    ZEntityRef s_SpawnerEntity, s_KeyEntity;
    SExternalReferences s_DummyExternalRefs;

    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        s_SpawnerEntity,
        "",
        s_SpawnerResource,
        s_pScene.m_ref,
        s_DummyExternalRefs,
        -1);
    Functions::ZEntityManager_NewEntity->Call(
        Globals::EntityManager,
        s_KeyEntity,
        "",
        s_KeyEntityResource,
        s_pScene.m_ref,
        s_DummyExternalRefs,
        -1);

    if (!s_SpawnerEntity || !s_KeyEntity)
    {
        Logger::Debug(TAG "entity spawn failed");
        return false;
    }

    const auto s_pItemSpawner = s_SpawnerEntity.QueryInterface<ZItemSpawner>();
    const auto s_KeyEntityRef = TEntityRef<ZItemRepositoryKeyEntity>(s_KeyEntity);
    if (!s_pItemSpawner || !s_KeyEntityRef)
    {
        Logger::Debug(TAG "failed to query interfaces of entities");
        return false;
    }

    s_pItemSpawner->m_ePhysicsMode = ZItemSpawner::EPhysicsMode::EPM_DYNAMIC; // fall down
    s_pItemSpawner->m_rMainItemKey = s_KeyEntityRef;
    s_pItemSpawner->m_bUsePlacementAttach = false;
    s_pItemSpawner->SetObjectToWorldMatrixFromEditor(s_Transform);

    s_KeyEntityRef.m_pInterfaceRef->m_RepositoryId = p_RepositoryId;

    Functions::ZItemSpawner_RequestContentLoad->Call(s_pItemSpawner);

    Logger::Debug(TAG "item spawner spawned");
    return true;
}

bool ZSpawnRandomItemEffect::LoadRepositoryProps()
{
    if (g_aRepositoryProps.size() > 0)
    {
        Logger::Debug(TAG "repository props already loaded, skipping load");
        return true;
    }

    const auto s_RepoID = ResId<"[assembly:/repository/pro.repo].pc_repo">;
    TResourcePtr<ZTemplateEntityFactory> s_RepositoryResource;
    Globals::ResourceManager->GetResourcePtr(s_RepositoryResource, s_RepoID, 0);

    if (!s_RepositoryResource || s_RepositoryResource.GetResourceInfo().status != RESOURCE_STATUS_VALID)
    {
        Logger::Debug(TAG "pro.repo not loaded");
        return false;
    }

    const auto s_mRepositoryData = static_cast<THashMap<ZRepositoryID, ZDynamicObject, TDefaultHashMapPolicy<ZRepositoryID>> *>(s_RepositoryResource.GetResourceData());
    for (const auto &[s_RepositoryId, s_Obj] : *s_mRepositoryData)
    {
        const auto *s_pEntries = s_Obj.As<TArray<SDynamicObjectKeyValuePair>>();

        std::string s_sId, s_sTitle, s_sCommonName, s_sName;
        bool s_bIsItem = false;
        for (const auto &s_Entry : *s_pEntries)
        {
            const std::string s_sKey = std::string(s_Entry.sKey.c_str(), s_Entry.sKey.size());

            if (s_sKey == "ID_")
                s_sId = DynamicObjectToString(s_Entry.value);
            else if (s_sKey == "Title")
                s_sTitle = DynamicObjectToString(s_Entry.value);
            else if (s_sKey == "CommonName")
                s_sCommonName = DynamicObjectToString(s_Entry.value);
            else if (s_sKey == "Name")
                s_sName = DynamicObjectToString(s_Entry.value);
            else if (s_sKey == "ItemType")
                s_bIsItem = true;

            if (s_sId.empty() || !s_bIsItem)
            {
                continue;
            }

            std::string s_sFinalName;
            if (!s_sTitle.empty())
                s_sFinalName = s_sTitle;
            else if (!s_sCommonName.empty())
                s_sFinalName = s_sCommonName;
            else if (!s_sName.empty())
                s_sFinalName = s_sName;
            else
                s_sFinalName = "<unnamed>";

            s_sFinalName += " [" + s_sId + "]";

            g_aRepositoryProps.push_back(std::make_pair(
                ZRepositoryID(s_sId),
                s_sFinalName));
        }
    }

    Logger::Debug(TAG "Loaded {} repository items.", g_aRepositoryProps.size());
    return g_aRepositoryProps.size() > 0;
}

std::string ZSpawnRandomItemEffect::DynamicObjectToString(const ZDynamicObject &p_DynamicObject)
{
    const auto *s_pType = p_DynamicObject.GetTypeID()->typeInfo();
    const auto s_sTypeName = std::string(s_pType->m_pTypeName);

    if (s_sTypeName == "ZString")
    {
        const auto *s_pValue = p_DynamicObject.As<ZString>();
        return s_pValue->c_str();
    }

    // don't care about any other types for now
    return "";
}

REGISTER_CHAOS_EFFECT_PARAM(CNT1, ZSpawnRandomItemEffect, "Loot Box", 1);
REGISTER_CHAOS_EFFECT_PARAM(CNT2, ZSpawnRandomItemEffect, "Deluxe Loot Box", 5);
REGISTER_CHAOS_EFFECT_PARAM(CNT3, ZSpawnRandomItemEffect, "Super Deluxe Loot Box", 10);
