#include "PlayerUtils.h"

#include "IPluginInterface.h"

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZContentKitManager.h>

#include <Logging.h>

#include "Helpers/EntityUtils.h"

bool Utils::TeleportPlayerTo(const SMatrix p_Position)
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player.m_entityRef)
    {
        return false;
    }

    auto s_PlayerSpatialEntity = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_PlayerSpatialEntity)
    {
        return false;
    }

    s_PlayerSpatialEntity->SetObjectToWorldMatrixFromEditor(p_Position);
    return true;
}

bool Utils::SetPlayerOutfit(const std::string &p_sCommonName, TEntityRef<ZGlobalOutfitKit> &p_OutfitOut)
{
    const auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return false;
    }

    const auto *s_pContentKitManager = Globals::ContentKitManager;
    if (!s_pContentKitManager)
    {
        return false;
    }

    for (const auto &s_KV : s_pContentKitManager->m_repositoryGlobalOutfitKits)
    {
        const auto &s_OutfitKit = s_KV.second;
        if (!s_OutfitKit || !s_OutfitKit.m_pInterfaceRef)
        {
            continue;
        }

        const auto s_sOutfitCommonName = s_OutfitKit.m_pInterfaceRef->m_sCommonName;
        if (p_sCommonName == s_sOutfitCommonName.c_str())
        {
            p_OutfitOut = s_OutfitKit;
            break;
        }
    }

    if (!p_OutfitOut || !p_OutfitOut.m_pInterfaceRef)
    {
        Logger::Error("SetPlayerOutfit: No outfit kit found with common name '{}'", p_sCommonName);
        return false;
    }

    Functions::ZHitman5_SetOutfit->Call(
        s_Player.m_pInterfaceRef,
        p_OutfitOut,
        0,
        0,
        false,
        false);

    return true;
}

TEntityRef<ZSpatialEntity> Utils::GetPlayerHeadAttachEntity(const TEntityRef<ZHitman5> p_rPlayer)
{
    if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(p_rPlayer.m_entityRef))
    {
        if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x0ff5798a35665af2 /* "HEAD" */); s_nIdx != -1)
        {
            if (auto* s_pHead = s_pBlueprint->GetSubEntity(p_rPlayer.m_entityRef.m_pObj, s_nIdx); s_pHead != nullptr)
            {
                return TEntityRef<ZSpatialEntity>(ZEntityRef(s_pHead));
            }
        }
    }

    return {};
}
