#include "ZAuthoritativeFigureEffect.h"

#include "Globals.h"


#include "EffectRegistry.h"
#include "Helpers/Utils.h"

#define TAG "[ZAuthoritativeFigureEffect] "

ZAuthoritativeFigureEffect::ZAuthoritativeFigureEffect() : m_bOriginalAuthorityFigure(false), m_bOriginalWeaponsAllowed(false)
{
    m_aOutfitCommonNames = {
        "CHAR_Reward_Chlorine_Hero_M_Flamingo_Suit_PRO167217", // Flamingo
        "CHAR_GOTY_Hero_ClownSuit_M_PRO81498",                 // Clown
        "CHAR_GOTY_Hero_CowboySuit_M_PRO81499",                // Cowboy
        "CHAR_Legacy_Hero_SantaClausSuit_M_HPA2809",           // Santa47
        "CHARSET_Agent47_Reward_Christmas_4",                  // Sweater w/ Reindeer Horns
        //"CHAR_Reward_Sapienza_Hero_SuperFan_PRO",              // "Super Fan" - Ghostly Mask
        "CHAR_Global_Hero_EasterDuck_M",                       // Weird Duck Mask
        "CHAR_Reward_Hero_HalloweenOutfit_M_PRO140008",        // Scarecrow w/ Pumpkin Head
        //"Paris_Unique_VampireMagician_M_HPA914",               // Vampire Magician (Paris)
        //"Stingray_Unique_PirateOutfit_M",                      // Pirate (Ambrose)
        "CHAR_Reward_Hero_Bellini_Eminem_M",                   // Eminem (Bellini Contract 12.2025)
    };
}

void ZAuthoritativeFigureEffect::Start()
{
    const auto s_sOutfitName = Utils::SelectRandomElement(m_aOutfitCommonNames);
    Logger::Debug(TAG "Selected outfit '{}'", s_sOutfitName);

    if (!Utils::SetPlayerOutfit(s_sOutfitName, m_SelectedOutfit))
    {
        Logger::Error(TAG "Failed to set outfit.");
        return;
    }

    m_bOriginalAuthorityFigure = m_SelectedOutfit.m_pInterfaceRef->m_bAuthorityFigure;
    m_bOriginalWeaponsAllowed = m_SelectedOutfit.m_pInterfaceRef->m_bWeaponsAllowed;

    // make outfit a authoritative figure
    m_SelectedOutfit.m_pInterfaceRef->m_bAuthorityFigure = true;
    m_SelectedOutfit.m_pInterfaceRef->m_bWeaponsAllowed = true;
}

void ZAuthoritativeFigureEffect::Stop()
{
    if (!m_SelectedOutfit)
    {
        return;
    }

    m_SelectedOutfit.m_pInterfaceRef->m_bAuthorityFigure = m_bOriginalAuthorityFigure;
    m_SelectedOutfit.m_pInterfaceRef->m_bWeaponsAllowed = m_bOriginalWeaponsAllowed;
}

bool ZAuthoritativeFigureEffect::Available()
{
    return IChaosEffect::Available() && Globals::ContentKitManager != nullptr;
}

REGISTER_CHAOS_EFFECT(ZAuthoritativeFigureEffect);
