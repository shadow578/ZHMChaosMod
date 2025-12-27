#include "ZNoHUDEffect.h"

#include "IPluginInterface.h"
#include "Logging.h"

#include "EffectRegistry.h"

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
    if (auto s_HUDRoot = SDK()->GetEntityById(c_nHUDRootId))
    {
        if (!s_HUDRoot.SetProperty(c_sHudVisibilityPropertyName, p_bVisible))
        {
            Logger::Debug(TAG "Failed to set visibility property on HUD root entity.");
        }
    }
}

REGISTER_CHAOS_EFFECT(ZNoHUDEffect);
