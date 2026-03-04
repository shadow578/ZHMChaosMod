#include "ZRatTestEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"

void ZRatTestEffect::LoadResources()
{
    m_pSpawner = ZTemplateEntitySpawner::Create <"[assembly:/_pro/characters/templates/animals/rats.template?/rat_a_white.entitytemplate].pc_entitytype">();
}

void ZRatTestEffect::OnClearScene()
{
    m_pSpawner = nullptr;
}

bool ZRatTestEffect::Available() const
{
    return IChaosEffect::Available()
           && m_pSpawner && m_pSpawner->IsAvailable();
}

void ZRatTestEffect::Start()
{
    auto s_rRat = m_pSpawner->SpawnAs<ZSpatialEntity>();

    auto s_rPlayer = SDK()->GetLocalPlayer();
    auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>();

    s_rRat.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_pPlayerSpatial->GetObjectToWorldMatrix());
}


void ZRatTestEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pSpawner->ToString()).c_str());
}


REGISTER_CHAOS_EFFECT(ZRatTestEffect);
