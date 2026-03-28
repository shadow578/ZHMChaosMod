#include "ZNoHUDEffect.h"

#include <Logging.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

#define TAG "[ZNoHUDEffect] "

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
    auto s_HUDRoot = Utils::ZEntityFinder()
                         .EntityID(EntityId::HM3::GameEssentials::MasterRoot)
                         .FindFirst();

    if (!s_HUDRoot)
    {
        return;
    }

    if (!Utils::SetProperty<bool>(s_HUDRoot, c_sHudVisibilityPropertyName, p_bVisible))
    {
        Logger::Debug(TAG "Failed to set visibility property on HUD root entity.");
    }
}

REGISTER_CHAOS_EFFECT(ZNoHUDEffect);
