#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/Enums.h>

// [modules:/zaimodifieractor.class].pc_entitytype
// [modules:/zaimodifierrole.class].pc_entitytype
struct SAIModifierBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(EAIModifierScope, m_nScope);
    PROPERTY(bool, m_bIgnoreLowNoise);
    PROPERTY(bool, m_bIgnoreSillyHitman);
    PROPERTY(bool, m_bIgnoreAnnoyingHitman);
    PROPERTY(bool, m_bIgnoreDistractions);
    PROPERTY(bool, m_bIgnoreAccidents);
    PROPERTY(bool, m_bIgnoreDeadBodies);
    PROPERTY(bool, m_bDisableDeadBodySensor);
    PROPERTY(bool, m_bIgnoreWeapons);
    PROPERTY(bool, m_bIgnoreTrespassing);
    PROPERTY(bool, m_bIgnoreLockdown);
    PROPERTY(bool, m_bDisableHelpCivilian);
    PROPERTY(bool, m_bNeverSpectate);
    PROPERTY(bool, m_bDeafAndBlind);
    PROPERTY(bool, m_bWantsPrivacy);
    PROPERTY(bool, m_bOneHitpoint);
    PROPERTY(bool, m_bBlockDeadlyThrow);
    PROPERTY(bool, m_bBlockMelee);
    PROPERTY(bool, m_bBlockDeath);
    PROPERTY(bool, m_bSuppressSocialGreeting);

    INPUT_PIN(Set);       // Set the modifiers
    INPUT_PIN(Clear);     // Clear the modifiers
};
