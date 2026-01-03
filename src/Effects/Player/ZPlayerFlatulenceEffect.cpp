#include "ZPlayerFlatulenceEffect.h"

#include "IPluginInterface.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

constexpr auto c_ridSFXFallback = ResId<"[assembly:/sound/wwise/exportedwwisedata/events/props_events/play_sfx_inflatable_deflate_01.wwiseevent].pc_wwisebank">;

void ZPlayerFlatulenceEffect::LoadResources() 
{
    ZPoisonAOEDamageEffectBase::LoadResources();
    ZSoundFXEffectBase::LoadResources();

	m_pSFXResource = ZResourceProvider::Create<"[assembly:/sound/wwise/exportedwwisedata/events/chaosmod/play_sfx_flatulence.wwiseevent].pc_wwisebank">();
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
        if (auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_Player.m_ref))
        {
            // position roughly at ass level when relative to player
            auto s_WM = SMatrix();
            s_WM.Trans.x = 0.0f;
            s_WM.Trans.y = 0.1f;
            s_WM.Trans.z = 0.9f;

            // poison AOE
            const ZPoisonAOEDamageEffectBase::SParams s_PoisonParams{
                .m_Position = s_WM,
                .m_eType = ZPoisonAOEDamageEffectBase::EPoisonType::SICK,
                .m_AreaSize = SVector3(8.0f, 8.0f, 8.0f),
                .m_ParticleColorRangeStart{.r = 25, .g = 240, .b = 0 },
                .m_ParticleColorRangeEnd{.r = 25, .g = 200, .b = 0 }
            };
            auto s_rPoisonEntity = ZPoisonAOEDamageEffectBase::Spawn(s_PoisonParams);

            // sound effect
            auto s_rSFXEntity = ZSoundFXEffectBase::PlayAt(s_WM, 
                m_pSFXResource->IsAvailable() ? m_pSFXResource->GetResourceID() : c_ridSFXFallback
            );

            // set player as parent of entities
			Utils::SetProperty<TEntityRef<ZSpatialEntity>>(s_rPoisonEntity, "m_eidParent", s_rPlayerSpatial);
			Utils::SetProperty<TEntityRef<ZSpatialEntity>>(s_rSFXEntity, "m_eidParent", s_rPlayerSpatial);
        }
    }
}

REGISTER_CHAOS_EFFECT(ZPlayerFlatulenceEffect)
