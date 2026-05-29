#pragma once
#include <Glacier/Enums.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZOutfit.h>
#include <Glacier/ZMath.h>

class ZSpatialEntity;
class ZHitman5;

namespace Utils
{
    /**
     * Get the local player entity.
     * Equivalent to Utils::GetLocalPlayer().
     */
    TEntityRef<ZHitman5> GetLocalPlayer();

    /**
     * Get the player's current transform. 
     */
    bool GetPlayerTransform(SMatrix& p_mTransform);

    /**
     * Teleport the player.
     * @param p_mTransform The new transform for the player.
     * @param p_mOldTransform Output parameter to receive the player's old transform
     * @param p_bPreserveRotation If true, the player's current rotation and scale (XAxis, YAxis, ZAxis) will be preserved and only the position (Pos) from p_mTransform will be applied.
     */
    bool TeleportPlayer(const SMatrix p_mTransform, SMatrix& p_mOldTransform, const bool p_bPreserveRotation = false);
    inline bool TeleportPlayer(const SMatrix p_mTransform, const bool p_bPreserveRotation = false)
    {
        SMatrix s_mOldTransform;
        return TeleportPlayer(p_mTransform, s_mOldTransform, p_bPreserveRotation);
    }
    inline bool TeleportPlayer(const float4 p_vPosition)
    {
        // construct matrix for teleport.
        // NOTE: with p_bPreserveRotation=true, only Pos is applied, so the values of XAxis/YAxis/ZAxis don't matter.
        SMatrix s_mTransform = SMatrix();
        s_mTransform.Pos = p_vPosition;
        return TeleportPlayer(s_mTransform, true);
    }

    /**
     * Set player outfit by common name.
     * @param p_sCommonName Outfit common name (e.g. "CHAR_Reward_Chlorine_Hero_M_Flamingo_Suit_PRO167217").
     *                      Only Outfits with CharacterType "HeroA" are supported.
     * @param p_OutfitOut Set set to the OutfitKit that was applied to the player, assuming the function returned true.
     */
    bool SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit>& p_OutfitOut);

    /**
     * Get the head attachment point entity for the player.
     * @param p_rPlayer Player to get head attach for.
     * @return Entity ref of head attach, or empty if not found.
     */
    TEntityRef<ZSpatialEntity> GetPlayerHeadAttachEntity(const TEntityRef<ZHitman5> p_rPlayer);
}; // namespace Utils