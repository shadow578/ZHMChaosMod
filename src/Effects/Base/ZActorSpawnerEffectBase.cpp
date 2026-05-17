#include "ZActorSpawnerEffectBase.h"

#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/PlayerUtils.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZActorSpawnerEffectBase] "

void ZActorSpawnerEffectBase::LoadResources()
{
    m_pActorSpawner = ZTemplateEntitySpawner::Create<"[assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype">();
    ZHMRepositoryHelper::GetInstance().Initialize();
}

void ZActorSpawnerEffectBase::OnClearScene()
{
    m_pActorSpawner = nullptr;
    ZHMRepositoryHelper::GetInstance().Reset();
}

bool ZActorSpawnerEffectBase::Available() const
{
    return IChaosEffect::Available()
           && m_pActorSpawner && m_pActorSpawner->IsAvailable()
           && ZHMRepositoryHelper::GetInstance().IsLoaded();
}

void ZActorSpawnerEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pActorSpawner->ToString()).c_str());
}

TEntityRef<ZActor> ZActorSpawnerEffectBase::SpawnActor(const SMatrix m_mPosition, const std::string& p_sName, const std::string& p_sOutfitRepositoryId, const uint8_t p_nCharsetIndex, const uint8_t p_nOutfitVariationIndex)
{
    if (!m_pActorSpawner || !m_pActorSpawner->IsAvailable())
    {
        return {};
    }

    const ZRepositoryID s_ridOutfit(p_sOutfitRepositoryId);
    if (s_ridOutfit == ZRepositoryID())
    {
        return {};
    }

    const auto s_OutfitRepositoryEntry = ZHMRepositoryHelper::GetInstance().Get(s_ridOutfit);
    if (!s_OutfitRepositoryEntry)
    {
        return {};
    }

    const std::optional<ZString*> s_opsOutfitCommonName = s_OutfitRepositoryEntry->Get<ZString>("CommonName");
    if (!s_opsOutfitCommonName.has_value())
    {
        return {};
    }
    const std::string s_sOutfitCommonName = s_opsOutfitCommonName.value()->c_str();

    const auto s_rActor = m_pActorSpawner->SpawnAs<ZActor>();

    s_rActor.m_pInterfaceRef->m_sActorName = ZString(p_sName);
    s_rActor.m_pInterfaceRef->m_bStartEnabled = true;
    s_rActor.m_pInterfaceRef->m_nOutfitCharset = p_nCharsetIndex;
    s_rActor.m_pInterfaceRef->m_nOutfitVariation = p_nOutfitVariationIndex;
    s_rActor.m_pInterfaceRef->m_OutfitRepositoryID = s_ridOutfit;
    s_rActor.m_pInterfaceRef->m_eRequiredVoiceVariation = EActorVoiceVariation::eAVV_Undefined;

    s_rActor.m_pInterfaceRef->Activate(0);

    if (const auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(s_rActor.m_entityRef))
    {
        s_rActorSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(m_mPosition);
    }
    else
    {
        m_pActorSpawner->Despawn(s_rActor.m_entityRef);
        return {};
    }

    if (Utils::SetActorOutfit(s_rActor, s_sOutfitCommonName, p_nCharsetIndex, p_nOutfitVariationIndex))
    {
        return s_rActor;
    }

    m_pActorSpawner->Despawn(s_rActor.m_entityRef);
    return {};
}
