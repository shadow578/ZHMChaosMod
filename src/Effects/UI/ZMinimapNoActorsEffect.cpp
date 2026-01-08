#include "ZMinimapNoActorsEffect.h"

#include "IPluginInterface.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZMinimapNoActorsEffect] "

/**
 * Static Entity ID for Minimap entity.
 * There should only ever be one of these, so we can just lookup by ID.
 * assembly:_PRO/scenes/bricks/gameessentials.brick, UI>IngameMenu>MapMenu>InGameMapSetup>MiniMap
 */
constexpr uint64_t c_nMinimapRootId = 0xccda0a613b5d88cb;

const std::string c_sMinimapTypePropertyName = "m_eMapType";

void ZMinimapNoActorsEffect::Start()
{
	SetMinimapType(EMapType::E_MAPTYPE_MenuMap);
}

void ZMinimapNoActorsEffect::Stop()
{
	SetMinimapType(EMapType::E_MAPTYPE_Minimap);
}

void ZMinimapNoActorsEffect::SetMinimapType(const EMapType p_eMapType)
{
    auto s_rMinimapRoot = Utils::ZEntityFinder()
        .EntityID(c_nMinimapRootId)
		.FindFirst();

    if (!s_rMinimapRoot || !Utils::SetProperty<EMapType>(s_rMinimapRoot, c_sMinimapTypePropertyName, p_eMapType))
    {
        Logger::Debug(TAG "Failed to set minimap type.");
    }
}

REGISTER_CHAOS_EFFECT(ZMinimapNoActorsEffect)
