#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/Enums.h>

struct SAIModifierRoleBinding : SEntityBinding
{
    PROPERTY(bool, m_bIgnoreLowNoise);
    PROPERTY(bool, m_bIgnoreSillyHitman);
    PROPERTY(bool, m_bIgnoreAnnoyingHitman);
    PROPERTY(bool, m_bIgnoreDistractions);
    PROPERTY(bool, m_bIgnoreAccidents);
    PROPERTY(bool, m_bNeverSpectate);
    PROPERTY(bool, m_bConversationHelper);
    PROPERTY(bool, m_bConversationHelperFast);
    PROPERTY(bool, m_bWantsPrivacy);
    PROPERTY(bool, m_bOneHitpoint);
    PROPERTY(bool, m_bBlockDeadlyThrow);
    PROPERTY(bool, m_bBlockMelee);
    PROPERTY(bool, m_bSuppressSocialGreeting);
};
