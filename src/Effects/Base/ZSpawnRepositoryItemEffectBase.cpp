#include "ZSpawnRepositoryItemEffectBase.h"

#include <Glacier/ZContentKitManager.h>

#include <Logging.h>

#include "Helpers/Math.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZSpawnRepositoryItemEffectBase] "

std::vector<ZSpawnRepositoryItemEffectBase::SRepositoryPropInfo> ZSpawnRepositoryItemEffectBase::m_aRepositoryProps;

void ZSpawnRepositoryItemEffectBase::LoadResources()
{
    m_pItemSpawnerSpawner = ZTemplateEntitySpawner::Create<"[modules:/zitemspawner.class].pc_entitytype">();
    m_pRepositoryKeywordSpawner = ZTemplateEntitySpawner::Create<"[modules:/zitemrepositorykeyentity.class].pc_entitytype">();

	m_aRepositoryProps.clear();

    auto& s_Repo = ZHMRepositoryHelper::GetInstance();
    s_Repo.Initialize();

	for (const auto& s_RepositoryId : s_Repo.GetEntryIdsByType(ZHMRepositoryHelper::EEntryType::Item))
    {
		PushRepositoryProp(s_RepositoryId);
    }

    for (const auto& s_RepositoryId : s_Repo.GetEntryIdsByType(ZHMRepositoryHelper::EEntryType::Weapon))
    {
        PushRepositoryProp(s_RepositoryId);
    }

    Logger::Debug(TAG "Loaded {} repository items.", m_aRepositoryProps.size());
}

void ZSpawnRepositoryItemEffectBase::PushRepositoryProp(const ZRepositoryID& p_RepositoryId)
{
    const auto s_Obj = ZHMRepositoryHelper::GetInstance().Get(p_RepositoryId);

    std::string s_sDisplayName, s_sCommonName;
    if (const auto s_sDisplayNameOpt = s_Obj->Get<ZString>("Title"); s_sDisplayNameOpt.has_value())
    {
        s_sCommonName = s_sDisplayNameOpt.value()->c_str();
    }

    if (const auto s_sCommonNameOpt = s_Obj->Get<ZString>("CommonName"); s_sCommonNameOpt.has_value())
    {
        s_sCommonName = s_sCommonNameOpt.value()->c_str();
    }

    m_aRepositoryProps.push_back({
        .m_RepositoryID = p_RepositoryId,
        .m_sCommonName = s_sCommonName,
        .m_sDisplayName = s_sDisplayName
        });
}

void ZSpawnRepositoryItemEffectBase::OnClearScene()
{
    ZHMRepositoryHelper::GetInstance().Reset();

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

    s_rKeywordEntity.m_pInterfaceRef->m_RepositoryId = p_RepositoryProp.m_RepositoryID;

    //Functions::ZItemSpawner_RequestContentLoad->Call(s_rItemSpawner.m_pInterfaceRef);
    s_rItemSpawner.m_entityRef.SignalInputPin("SpawnItem");

    Logger::Debug(TAG "spawned item '{}' [{}] ({}).",
        p_RepositoryProp.m_sDisplayName,
        p_RepositoryProp.m_RepositoryID.ToString().c_str(),
        p_RepositoryProp.m_sCommonName
    );
    return true;
}

ZSpawnRepositoryItemEffectBase::SRepositoryPropInfo ZSpawnRepositoryItemEffectBase::GetRepositoryPropByID(const std::string& p_sID) const
{
	const auto s_RepositoryID = ZRepositoryID(p_sID);

    for (const auto& s_Prop : m_aRepositoryProps)
    {
        if (s_Prop.m_RepositoryID == s_RepositoryID)
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
