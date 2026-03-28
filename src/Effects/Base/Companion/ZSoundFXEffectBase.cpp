#include "ZSoundFXEffectBase.h"

#include <Logging.h>
#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZSoundFXEffectBase] "

void ZSoundFXEffectBase::LoadResources()
{
    m_pSoundPlayerSpawner = ZTemplateEntitySpawner::Create<"[assembly:/_pro/chaosmod/sfxplayer.entitytemplate].pc_entitytype">();
}

void ZSoundFXEffectBase::OnClearScene()
{
    m_pSoundPlayerSpawner = nullptr;
}

bool ZSoundFXEffectBase::Available() const
{
    return ZCompanionModDependentEffectBase::Available() && m_pSoundPlayerSpawner && m_pSoundPlayerSpawner->IsAvailable();
}

void ZSoundFXEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pSoundPlayerSpawner->ToString()).c_str());

    ImGui::BeginDisabled(!Available());

    if (ImGui::Button("Play Test SFX"))
    {
        if (const auto s_Player = Utils::GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetObjectToWorldMatrix();

                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 5.0f;

                const auto s_RuntimeResourceId = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/item_events/sfx_explosives/proximityexplosive_rubberduck/play_duckarmed.wwiseevent].pc_wwisebank">;
                PlayAt(s_WM, s_RuntimeResourceId, m_bDebugAsMusic);
            }
        }
    }

    ImGui::Checkbox("DBG Plays as Music", &m_bDebugAsMusic);

    ImGui::EndDisabled();
}

SSoundFXPlayerEntityBinding ZSoundFXEffectBase::PlayAt(const SMatrix& p_mPosition, const ZRuntimeResourceID& p_ridSound, const bool p_bMusic)
{
    auto s_Player = CreatePlayer(p_ridSound);
    if (!s_Player)
    {
        return {};
    }

    
    if (auto s_rSpatial = s_Player.QuerySpatial())
    {
        s_rSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(p_mPosition);
    }

    if (p_bMusic)
    {
        s_Player.StartMusic();
    }
    else
    {
        s_Player.StartSFX();
    }

    return s_Player;
}

SSoundFXPlayerEntityBinding ZSoundFXEffectBase::CreatePlayer(const ZRuntimeResourceID& p_ridSound)
{
    if (!m_pSoundPlayerSpawner)
    {
        return {};
    }

    auto s_Player = SSoundFXPlayerEntityBinding(m_pSoundPlayerSpawner->Spawn());
    if (!s_Player)
    {
        Logger::Debug(TAG "Failed to spawn SFX player entity.");
        return {};
    }

    s_Player.m_pMainEvent = p_ridSound;
    return s_Player;
}
