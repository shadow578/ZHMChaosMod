#pragma once

#include "Globals.h"

#include "Glacier/ZActor.h"
#include "Glacier/ZMath.h"

namespace Utils
{
    template <typename T>
    static T* CastRaw(void* p_pData)
    {
        return reinterpret_cast<T*>(p_pData);
    }

    template <typename T>
    static T GetRandomNumber(const T p_Min, const T p_Max)
    {
        static_assert(std::is_arithmetic<T>::value, "GetRandomNumber only supports arithmetic types.");

        if constexpr (std::is_integral<T>::value)
        {
            const auto s_Range = p_Max - p_Min + 1;
            const auto s_RandomValue = rand() % s_Range;
            return p_Min + s_RandomValue;
        }
        else
        {
            const auto s_RandomValue = static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
            return p_Min + s_RandomValue * (p_Max - p_Min);
        }
    }

    template <typename T>
    static T SelectRandomElement(const std::vector<T>& p_aElements)
    {
        if (p_aElements.empty())
        {
            throw std::runtime_error("SelectRandomElement called with empty vector.");
        }

        const auto s_Index = GetRandomNumber<size_t>(0, p_aElements.size() - 1);
        return p_aElements[s_Index];
    }

    template <typename T>
    static T Interpolate(const T& p_A, const T& p_B, const float32 p_fT)
    {
        if (p_fT <= 0.0f)
        {
            return p_A;
        }
        else if (p_fT >= 1.0f)
        {
            return p_B;
        }

        return p_A + (p_B - p_A) * p_fT;
    }

    template <typename Base, typename T>
    static bool IsInstanceOf(const T* p_pOther)
    {
        return dynamic_cast<const Base*>(p_pOther) != nullptr;
    }

    SMatrix InterpolateAffine(const SMatrix& p_A, const SMatrix& p_B, const float32 p_fT);

    std::vector<ZActor*> GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified);

    ZActor* GetRandomActor(const bool p_bRequireAlive);

    bool TeleportPlayerTo(const SMatrix p_Position);

    /**
     * Set player outfit by common name (e.g. "CHAR_Reward_Chlorine_Hero_M_Flamingo_Suit_PRO167217").
     * Only works for Outfits with CharacterType "HeroA".
     * p_OutfitOut will be set to the OutfitKit that was applied to the player, assuming the function returns true.
     */
    bool SetPlayerOutfit(const std::string& p_sCommonName, TEntityRef<ZGlobalOutfitKit> &p_OutfitOut);
};