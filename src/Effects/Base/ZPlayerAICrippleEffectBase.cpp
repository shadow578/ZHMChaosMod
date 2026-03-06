#include "ZPlayerAICrippleEffectBase.h"

#include <imgui.h>

void ZPlayerAICrippleEffectBase::LoadResources()
{
    m_pAICrippleSpawner = ZTemplateEntitySpawner::Create<"[modules:/zaicrippleentity.class].pc_entitytype">();
}

void ZPlayerAICrippleEffectBase::OnEnterScene()
{
    if (m_pAICrippleSpawner && m_pAICrippleSpawner->IsAvailable())
    {
        m_rAICripple = m_pAICrippleSpawner->Spawn();
    }
}

void ZPlayerAICrippleEffectBase::OnClearScene()
{
    m_rAICripple = {};
    m_pAICrippleSpawner = nullptr;
}

bool ZPlayerAICrippleEffectBase::Available() const
{
    return IChaosEffect::Available()
           && m_pAICrippleSpawner && m_pAICrippleSpawner->IsAvailable()
           && m_rAICripple;
}

void ZPlayerAICrippleEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pAICrippleSpawner->ToString()).c_str());
    ImGui::TextUnformatted(fmt::format("AICripple Entity: {}", m_rAICripple ? "Spawned" : "Not Spawned").c_str());
}
