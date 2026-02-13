#include "ZSpawnRepositoryItemEffectBase.h"

#include <Glacier/ZContentKitManager.h>

#include <Logging.h>

#include "Helpers/Math.h"

#define TAG "[ZSpawnRepositoryItemEffectBase] "

std::vector<ZSpawnRepositoryItemEffectBase::SRepositoryPropInfo> ZSpawnRepositoryItemEffectBase::m_aRepositoryProps;

void ZSpawnRepositoryItemEffectBase::LoadResources()
{
    m_pItemSpawnerSpawner = ZTemplateEntitySpawner::Create<"[modules:/zitemspawner.class].pc_entitytype">();
    m_pRepositoryKeywordSpawner = ZTemplateEntitySpawner::Create<"[modules:/zitemrepositorykeyentity.class].pc_entitytype">();

    m_bIsAvailable = LoadRepositoryProps();
}

void ZSpawnRepositoryItemEffectBase::OnClearScene()
{
    m_aRepositoryProps.clear();

    m_pItemSpawnerSpawner = nullptr;
    m_pRepositoryKeywordSpawner = nullptr;
}

bool ZSpawnRepositoryItemEffectBase::Available() const
{
    return IChaosEffect::Available()
        && m_pItemSpawnerSpawner && m_pItemSpawnerSpawner->IsAvailable()
        && m_pRepositoryKeywordSpawner && m_pRepositoryKeywordSpawner->IsAvailable()
        && m_aRepositoryProps.size() > 0;
}

void ZSpawnRepositoryItemEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Item Spawner Prop: {}", m_pItemSpawnerSpawner->ToString()).c_str());
    ImGui::TextUnformatted(fmt::format("Keyword Prop: {}", m_pRepositoryKeywordSpawner->ToString()).c_str());
    ImGui::TextUnformatted(fmt::format("# Repository Props Loaded: {}", m_aRepositoryProps.size()).c_str());
}

bool ZSpawnRepositoryItemEffectBase::SpawnRepositoryPropAt(const SRepositoryPropInfo& p_RepositoryProp, const float4 s_vPosition, const ZItemSpawner::EPhysicsMode p_ePhysicsMode)
{
    if (!p_RepositoryProp || !m_pItemSpawnerSpawner || !m_pRepositoryKeywordSpawner)
    {
        Logger::Debug(TAG "invalid parameters or spawners not initialized");
        return false;
    }

    const auto s_rItemSpawner = m_pItemSpawnerSpawner->SpawnAs<ZItemSpawner>();
    if (!s_rItemSpawner)
    {
        Logger::Debug(TAG "failed to spawn item spawner");
        return false;
    }

    const auto s_rKeywordEntity = m_pRepositoryKeywordSpawner->SpawnAs<ZItemRepositoryKeyEntity>();
    if (!s_rKeywordEntity)
    {
        Logger::Debug(TAG "failed to spawn repository keyword entity");
        m_pItemSpawnerSpawner->Despawn(s_rItemSpawner.m_entityRef);
        return false;
    }

    s_rItemSpawner.m_pInterfaceRef->m_ePhysicsMode = p_ePhysicsMode;
    s_rItemSpawner.m_pInterfaceRef->m_rMainItemKey = s_rKeywordEntity;
    s_rItemSpawner.m_pInterfaceRef->m_bUsePlacementAttach = false;

    SMatrix s_Transform = SMatrix();
    s_Transform.Trans = s_vPosition;
    s_rItemSpawner.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_Transform);

    s_rKeywordEntity.m_pInterfaceRef->m_RepositoryId = ZRepositoryID(p_RepositoryProp.m_sID);

    //Functions::ZItemSpawner_RequestContentLoad->Call(s_rItemSpawner.m_pInterfaceRef);
    s_rItemSpawner.m_entityRef.SignalInputPin("SpawnItem");

    Logger::Debug(TAG "spawned item '{}' [{}] ({}).",
        p_RepositoryProp.m_sDisplayName,
        p_RepositoryProp.m_sID,
        p_RepositoryProp.m_sCommonName
    );
    return true;
}

bool ZSpawnRepositoryItemEffectBase::LoadRepositoryProps()
{
    if (m_aRepositoryProps.size() > 0)
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

    const auto s_mRepositoryData = static_cast<THashMap<ZRepositoryID, ZDynamicObject, TDefaultHashMapPolicy<ZRepositoryID>>*>(s_RepositoryResource.GetResourceData());
    for (const auto& [s_RepositoryId, s_Obj] : *s_mRepositoryData)
    {
        const auto* s_pEntries = s_Obj.As<TArray<SDynamicObjectKeyValuePair>>();

        std::string s_sId, s_sTitle, s_sCommonName, s_sName;
        bool s_bIsItem = false;
        for (const auto& s_Entry : *s_pEntries)
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
        }

        if (s_sId.empty() || !s_bIsItem)
        {
            continue;
        }

        std::string s_sDisplayName;
        if (!s_sTitle.empty())
            s_sDisplayName = s_sTitle;
        else if (!s_sCommonName.empty())
            s_sDisplayName = s_sCommonName;
        else if (!s_sName.empty())
            s_sDisplayName = s_sName;
        else
            s_sDisplayName = "";

        SRepositoryPropInfo s_Info{
            .m_sID = s_sId,
            .m_sCommonName = s_sCommonName,
            .m_sDisplayName = s_sDisplayName
        };
        m_aRepositoryProps.push_back(s_Info);
    }

    Logger::Debug(TAG "Loaded {} repository items.", m_aRepositoryProps.size());
    return m_aRepositoryProps.size() > 0;
}

std::string ZSpawnRepositoryItemEffectBase::DynamicObjectToString(const ZDynamicObject& p_DynamicObject)
{
    const auto* s_pType = p_DynamicObject.GetTypeID()->GetTypeInfo();
    const auto s_sTypeName = std::string(s_pType->pszTypeName);

    if (s_sTypeName == "ZString")
    {
        const auto* s_pValue = p_DynamicObject.As<ZString>();
        return s_pValue->c_str();
    }

    // don't care about any other types for now
    return "";
}

ZSpawnRepositoryItemEffectBase::SRepositoryPropInfo ZSpawnRepositoryItemEffectBase::GetRepositoryPropByID(const std::string& p_sID) const
{
    for (const auto& s_Prop : m_aRepositoryProps)
    {
        if (s_Prop.m_sID == p_sID)
        {
            return s_Prop;
        }
    }

    return {};
}

ZSpawnRepositoryItemEffectBase::SRepositoryPropInfo ZSpawnRepositoryItemEffectBase::GetRepositoryPropByCommonName(const std::string& p_sCommonName) const
{
    for (const auto& s_Prop : m_aRepositoryProps)
    {
        if (s_Prop.m_sCommonName == p_sCommonName)
        {
            return s_Prop;
        }
    }

    return {};
}
