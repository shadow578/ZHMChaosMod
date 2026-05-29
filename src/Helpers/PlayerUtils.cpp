#include "PlayerUtils.h"

#include <IPluginInterface.h> // for SDK()
#include <Logging.h>

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZContentKitManager.h>

#include "Helpers/EntityUtils.h"
#include "Entity/EntityIds.h"

TEntityRef<ZHitman5> Utils::GetLocalPlayer()
{
    return SDK()->GetLocalPlayer();
}

bool Utils::GetPlayerTransform(SMatrix& p_mTransform)
{
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (!s_rPlayer.m_entityRef)
    {
        return false;
    }

    const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef);
    if (!s_rPlayerSpatial)
    {
        return false;
    }

    p_mTransform = s_rPlayerSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    return true;
}

bool Utils::TeleportPlayer(const SMatrix p_mTransform, SMatrix& p_mOldTransform, const bool p_bPreserveRotation)
{
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (!s_rPlayer.m_entityRef)
    {
        return false;
    }

    const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef);
    if (!s_rPlayerSpatial)
    {
        return false;
    }

    auto s_mTransform = s_rPlayerSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    p_mOldTransform = s_mTransform;

    if (p_bPreserveRotation)
    {
        s_mTransform.Pos = p_mTransform.Pos;
    }
    else
    {
        s_mTransform = p_mTransform;
    }

    // FIXME: this fails when the player is in certain actions (e.g. on a ledge)
    s_rPlayerSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mTransform);
    return true;
}

bool Utils::SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit>& p_OutfitOut)
{
    const auto s_Player = Utils::GetLocalPlayer();
    if (!s_Player)
    {
        return false;
    }

    const auto* s_pContentKitManager = Globals::ContentKitManager;
    if (!s_pContentKitManager)
    {
        return false;
    }

    for (const auto& s_KV : s_pContentKitManager->m_repositoryGlobalOutfitKits)
    {
        const auto& s_OutfitKit = s_KV.second;
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
        false
    );

    return true;
}

TEntityRef<ZSpatialEntity> Utils::GetPlayerHeadAttachEntity(const TEntityRef<ZHitman5> p_rPlayer)
{
    return TEntityRef<ZSpatialEntity>(GetSubEntity(p_rPlayer.m_entityRef, EntityId::HM3::Agent47Default::Head));
}
