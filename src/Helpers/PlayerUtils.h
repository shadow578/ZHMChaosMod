#pragma once
#include <Glacier/Enums.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZOutfit.h>
#include <Glacier/ZMath.h>

namespace Utils
{
    /**
     * Teleport the player.
     * @param p_Position Target transform to set.
     */
    bool TeleportPlayerTo(const SMatrix p_Position);

    /**
     * Set player outfit by common name.
     * @param p_sCommonName Outfit common name (e.g. "CHAR_Reward_Chlorine_Hero_M_Flamingo_Suit_PRO167217"). 
     *                      Only Outfits with CharacterType "HeroA" are supported.
     * @param p_OutfitOut Set set to the OutfitKit that was applied to the player, assuming the function returned true.
     */
    bool SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit>& p_OutfitOut);
};