#include "ZSeagullMayhemEffect.h"

#include <Logging.h>
#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"

#include "Entity/Bindings/SSeagullMayhemEntityBinding.h"

#define TAG "[ZSeagullMayhemEffect] "

void ZSeagullMayhemEffect::LoadResources()
{
    m_pEffectSpawner = ZTemplateEntitySpawner::Create<"[assembly:/_pro/chaosmod/seagull_mayhem.entitytemplate].pc_entitytype">();
}

void ZSeagullMayhemEffect::OnClearScene()
{
    m_pEffectSpawner = nullptr;
}

bool ZSeagullMayhemEffect::Available() const
{
    return ZCompanionModDependentEffectBase::Available() && m_pEffectSpawner && m_pEffectSpawner->IsAvailable();
}

void ZSeagullMayhemEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pEffectSpawner->ToString()).c_str());
}

void ZSeagullMayhemEffect::Start()
{
    // there's no way to manually despawn the effect's particles, so instead we start in OnSlowUpdate
    // and set the particle time to live to the remaining effect time, so that they automatically despawn when the effect ends.
    m_bDoSpawnOnNextUpdate = true;
}

void ZSeagullMayhemEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (!m_bDoSpawnOnNextUpdate)
    {
        return;
    }

    m_bDoSpawnOnNextUpdate = false;

    if (const auto s_rPlayer = Utils::GetLocalPlayer())
    {
        if (const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef))
        {
            if (auto s_rEffectEntity = m_pEffectSpawner->Spawn())
            {
                auto s_Binding = SSeagullMayhemBinding(s_rEffectEntity);

                // parent to player
                s_Binding.m_eidParent = s_rPlayerSpatial;

                // set effect TTL to remaining effect time, or at least 10 seconds
                auto s_fEffectTTL = p_fEffectTimeRemaining > 10.0f ? p_fEffectTimeRemaining : 10.0f;
                s_Binding.m_fTimeToLiveSeconds = s_fEffectTTL;

                // go!
                s_Binding.Activate();
            }
        }
    }
}

REGISTER_CHAOS_EFFECT(ZSeagullMayhemEffect);
