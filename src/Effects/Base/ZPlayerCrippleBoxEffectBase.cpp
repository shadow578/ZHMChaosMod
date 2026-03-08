#include "ZPlayerCrippleBoxEffectBase.h"

#include <Logging.h>
#include <imgui.h>

#include <Glacier/ZHM5CrippleBox.h>

#include "Helpers/PlayerUtils.h"

#define TAG "[ZPlayerCrippleBoxEffectBase] "

void ZPlayerCrippleBoxEffectBase::LoadResources()
{
    m_pCrippleBoxSpawner = ZTemplateEntitySpawner::Create<"[modules:/zhm5cripplebox.class].pc_entitytype">();
}

void ZPlayerCrippleBoxEffectBase::OnEnterScene()
{
    if (m_pCrippleBoxSpawner && m_pCrippleBoxSpawner->IsAvailable())
    {
        m_rCrippleBox = m_pCrippleBoxSpawner->SpawnAs<ZHM5CrippleBox>();
    }
}

void ZPlayerCrippleBoxEffectBase::OnClearScene()
{
    m_rCrippleBox = {};
    m_pCrippleBoxSpawner = nullptr;
}

bool ZPlayerCrippleBoxEffectBase::Available() const
{
    return IChaosEffect::Available()
           && m_pCrippleBoxSpawner && m_pCrippleBoxSpawner->IsAvailable()
           && m_rCrippleBox;
}

void ZPlayerCrippleBoxEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pCrippleBoxSpawner->ToString()).c_str());
    ImGui::TextUnformatted(fmt::format("CrippleBox Entity: {}", m_rCrippleBox ? "Spawned" : "Not Spawned").c_str());
}

void ZPlayerCrippleBoxEffectBase::Stop()
{
    m_rCrippleBox.m_pInterfaceRef->Deactivate(0);
}

void ZPlayerCrippleBoxEffectBase::ActivateCrippleBox()
{
    if (const auto s_rPlayer = Utils::GetLocalPlayer())
    {
        m_rCrippleBox.m_pInterfaceRef->m_bActivateOnStart = true;
        m_rCrippleBox.m_pInterfaceRef->m_rHitmanCharacter = s_rPlayer;

        m_rCrippleBox.m_pInterfaceRef->Activate(0);
    }
    else
    {
        Logger::Warn(TAG "::ActivateCrippleBox() could not activate, failed to get player reference.");
    }
}
