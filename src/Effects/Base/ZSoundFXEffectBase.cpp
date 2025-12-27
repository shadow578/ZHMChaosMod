#include "ZSoundFXEffectBase.h"

#include "IPluginInterface.h"
#include <Logging.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZEntityResourceSpawner.h"

#define TAG "[ZSoundFXEffectBase] "

static ZEntityResourceSpawner<"[assembly:/_pro/chaosmod/sfxplayer.entitytemplate].pc_entitytype"> g_SFXPlayerProp;

void ZSoundFXEffectBase::LoadResources()
{
	m_bIsAvailable = g_SFXPlayerProp.IsAvailable();
}

void ZSoundFXEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", g_SFXPlayerProp.ToString()).c_str());

    ImGui::BeginDisabled(!g_SFXPlayerProp.IsAvailable(false));

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

void ZSoundFXEffectBase::PlayAt(const SMatrix& p_Position, const ZRuntimeResourceID& p_SoundResource)
{
	auto s_RootEntity = g_SFXPlayerProp.SpawnAs<ZSpatialEntity>();
    if (!s_RootEntity)
    {
        Logger::Debug(TAG "Failed to spawn SFX player entity.");
        return;
    }

    s_RootEntity.m_pInterfaceRef->SetWorldMatrix(p_Position);
    s_RootEntity.m_ref.SetProperty("m_pMainEvent", p_SoundResource);

    s_RootEntity.m_ref.SignalInputPin("Start");
}
