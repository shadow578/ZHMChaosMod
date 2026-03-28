#include "ZMinimapNoActorsEffect.h"

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

#define TAG "[ZMinimapNoActorsEffect] "

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
    // There should only ever be one entity with this ID, so we can just find the first result.
    auto s_rMinimapRoot = Utils::ZEntityFinder()
                              .EntityID(EntityId::HM3::GameEssentials::Minimap)
                              .FindFirst();

    if (!s_rMinimapRoot || !Utils::SetProperty<EMapType>(s_rMinimapRoot, c_sMinimapTypePropertyName, p_eMapType))
    {
        Logger::Debug(TAG "Failed to set minimap type.");
    }
}

REGISTER_CHAOS_EFFECT(ZMinimapNoActorsEffect)
