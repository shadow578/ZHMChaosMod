#include "ZPlayerFlatulenceEffect.h"

#include "IPluginInterface.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"

void ZPlayerFlatulenceEffect::LoadResources() 
{
    ZPoisonAOEDamageEffectBase::LoadResources();
    ZSoundFXEffectBase::LoadResources();
}

void ZPlayerFlatulenceEffect::OnDrawDebugUI() 
{
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
                .m_AreaSize = SVector3(5.0f, 5.0f, 5.0f),
                .m_ParticleColorRangeStart{.r = 69, .g = 191, .b = 0 },
                .m_ParticleColorRangeEnd{.r = 94, .g = 255, .b = 0 }
            };
            ZPoisonAOEDamageEffectBase::Spawn(s_PoisonParams);

            // sound effect
            constexpr auto s_SoundResourceId = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/props_events/play_sfx_inflatable_deflate_01.wwiseevent].pc_wwisebank">;
            ZSoundFXEffectBase::PlayAt(s_WM, s_SoundResourceId);
        }
    }
}

REGISTER_CHAOS_EFFECT(ZPlayerFlatulenceEffect)
