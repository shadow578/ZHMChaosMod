#include "ZMinimapNoActorsEffect.h"

#include "IPluginInterface.h"

#include "EffectRegistry.h"

/**
 * Static Entity ID for Minimap entity.
 * There should only ever be one of these, so we can just lookup by ID.
 * assembly:_PRO/scenes/bricks/gameessentials.brick, UI>IngameMenu>MapMenu>InGameMapSetup>MiniMap
 */
constexpr uint64_t c_nMinimapRootId = 0xccda0a613b5d88cb;

const std::string c_sMinimapTypePropertyName = "m_eMapType";

void ZMinimapNoActorsEffect::Start()
{
    if (auto p_MinimapEntity = SDK()->GetEntityById(c_nMinimapRootId))
    {
        // MenuMap (and MainMap) = no NPCs, MiniMap = with 
        p_MinimapEntity.SetProperty(c_sMinimapTypePropertyName, EMapType::E_MAPTYPE_MenuMap);
    }
}

void ZMinimapNoActorsEffect::Stop()
{
    if (auto p_MinimapEntity = SDK()->GetEntityById(c_nMinimapRootId))
    {
        p_MinimapEntity.SetProperty(c_sMinimapTypePropertyName, EMapType::E_MAPTYPE_Minimap);
    }
}

REGISTER_CHAOS_EFFECT(ZMinimapNoActorsEffect)
