#include "ZPlayerFlatulenceEffect.h"

#include "IPluginInterface.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"

constexpr auto c_ridSFXFallback = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/props_events/play_sfx_inflatable_deflate_01.wwiseevent].pc_wwisebank">;
static ZResourceProvider<"[assembly:/sound/wwise/exportedwwisedata/events/chaosmod/play_sfx_flatulence.wwiseevent].pc_wwisebank"> g_SFXResourceProvider;

void ZPlayerFlatulenceEffect::LoadResources() 
{
    ZPoisonAOEDamageEffectBase::LoadResources();
    ZSoundFXEffectBase::LoadResources();

	m_pSFXResource = g_SFXResourceProvider.CreateSession();
}

void ZPlayerFlatulenceEffect::OnClearScene() 
{
    ZPoisonAOEDamageEffectBase::OnClearScene();
    ZSoundFXEffectBase::OnClearScene();

	m_pSFXResource = nullptr;
}

void ZPlayerFlatulenceEffect::OnDrawDebugUI() 
{
	ImGui::TextUnformatted(fmt::format("SFX Resource: {}", m_pSFXResource->ToString()).c_str());

    ImGui::SeparatorText("ZPoisonAOEDamageEffectBase");
    ZPoisonAOEDamageEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZSoundFXEffectBase");
    ZSoundFXEffectBase::OnDrawDebugUI();
}

bool ZPlayerFlatulenceEffect::Available() const
{
    return ZPoisonAOEDamageEffectBase::Available() &&
           ZSoundFXEffectBase::Available();
}

void ZPlayerFlatulenceEffect::Start()
{
    if (const auto s_Player = SDK()->GetLocalPlayer())
    {
        if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
        {
            auto s_WM = s_PlayerSpatial->GetWorldMatrix();

            // position roughly at ass level
            const auto s_Forward = (-s_WM.Backward).Normalized();
            const auto s_Upward = (s_WM.Up).Normalized();
            s_WM.Trans += (s_Upward * 0.60f) + (s_Forward * -0.20f);

            // poison AOE
            const ZPoisonAOEDamageEffectBase::SParams s_PoisonParams{
                .m_Position = s_WM,
                .m_eType = ZPoisonAOEDamageEffectBase::EPoisonType::SICK,
                .m_AreaSize = SVector3(8.0f, 8.0f, 8.0f),
                .m_ParticleColorRangeStart{.r = 69, .g = 191, .b = 0 },
                .m_ParticleColorRangeEnd{.r = 94, .g = 255, .b = 0 }
            };
            ZPoisonAOEDamageEffectBase::Spawn(s_PoisonParams);

            // sound effect
            ZSoundFXEffectBase::PlayAt(s_WM, 
                m_pSFXResource->IsAvailable() ? m_pSFXResource->GetResourceID() : c_ridSFXFallback
            );
        }
    }
}

REGISTER_CHAOS_EFFECT(ZPlayerFlatulenceEffect)
