#include "ZSoundFXEffectBase.h"

#include "IPluginInterface.h"
#include <Logging.h>

#include <Glacier/ZSpatialEntity.h>

#define TAG "[ZSoundFXEffectBase] "

static ZTemplateEntitySpawner<"[assembly:/_pro/chaosmod/sfxplayer.entitytemplate].pc_entitytype"> g_SFXPlayerProp;

void ZSoundFXEffectBase::LoadResources()
{
	m_pSoundPlayerSpawner = g_SFXPlayerProp.CreateSession();
}

void ZSoundFXEffectBase::OnClearScene()
{
    m_pSoundPlayerSpawner = nullptr;
}

bool ZSoundFXEffectBase::Available() const
{
    return IChaosEffect::Available() &&
        m_pSoundPlayerSpawner &&
        m_pSoundPlayerSpawner->IsAvailable();
}

void ZSoundFXEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pSoundPlayerSpawner->ToString()).c_str());

    ImGui::BeginDisabled(!Available());

    if (ImGui::Button("Play Test SFX"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 5.0f;

                const auto s_RuntimeResourceId = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/item_events/sfx_explosives/proximityexplosive_rubberduck/play_duckarmed.wwiseevent].pc_wwisebank">;
                PlayAt(s_WM, s_RuntimeResourceId);
            }
        }
    }

    ImGui::EndDisabled();
}

ZEntityRef ZSoundFXEffectBase::PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource)
{
    if (!m_pSoundPlayerSpawner)
    {
        return {};
    }

	auto s_RootEntity = m_pSoundPlayerSpawner->SpawnAs<ZSpatialEntity>();
    if (!s_RootEntity)
    {
        Logger::Debug(TAG "Failed to spawn SFX player entity.");
        return {};
    }

    s_RootEntity.m_pInterfaceRef->SetWorldMatrix(p_Position);
    s_RootEntity.m_ref.SetProperty("m_pMainEvent", p_SoundResource);

    s_RootEntity.m_ref.SignalInputPin("Start");

	return s_RootEntity.m_ref;
}
