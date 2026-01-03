#include "ZNoHUDEffect.h"

#include "IPluginInterface.h"
#include "Logging.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZNoHUDEffect] "

/**
 * Static Entity ID for HUD MASTER_ROOT entity.
 * assembly:_PRO/scenes/bricks/gameessentials.brick, UI>MASTER_ROOT
 */
constexpr uint64_t c_nHUDRootId = 0x77bed13890286553;

const std::string c_sHudVisibilityPropertyName = "m_bIsVisible";

void ZNoHUDEffect::Start()
{
    SetHUDVisibility(false);
}

void ZNoHUDEffect::Stop()
{
    SetHUDVisibility(true);
}

void ZNoHUDEffect::SetHUDVisibility(const bool p_bVisible)
{
    const Utils::EntityFinder::SSearchParams s_Query{
        .m_nEntityId = c_nHUDRootId,
        .m_nMaxResults = 1
    };
    auto s_aEntities = Utils::EntityFinder::FindEntities(s_Query);
    if (s_aEntities.empty())
    {
        return;
    }

    auto& s_HUDRoot = s_aEntities.front();
    if (!Utils::SetProperty<bool>(s_HUDRoot, c_sHudVisibilityPropertyName, p_bVisible))
    {
        Logger::Debug(TAG "Failed to set visibility property on HUD root entity.");
    }
}

REGISTER_CHAOS_EFFECT(ZNoHUDEffect);
