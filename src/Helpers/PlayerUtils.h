#pragma once
#include <Glacier/Enums.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZOutfit.h>
#include <Glacier/ZMath.h>

namespace Utils
{
    bool TeleportPlayerTo(const SMatrix p_Position);

    /**
     * Set player outfit by common name (e.g. "CHAR_Reward_Chlorine_Hero_M_Flamingo_Suit_PRO167217").
     * Only works for Outfits with CharacterType "HeroA".
     * p_OutfitOut will be set to the OutfitKit that was applied to the player, assuming the function returns true.
     */
    bool SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit>& p_OutfitOut);
};