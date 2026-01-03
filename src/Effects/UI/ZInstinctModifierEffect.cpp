#include "ZInstinctModifierEffect.h"

#include <Glacier/ZActor.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

const std::string c_GlowTypePropertyName = "m_eGlowType";

void ZInstinctModifierEffectBase::Start()
{
    m_mOriginalGlowTypes.clear();

    for(auto* s_pActor : Utils::GetActors(true, true))
    {
        if (!s_pActor)
        {
            continue;
        }

        auto s_rCharacter = s_pActor->m_rCharacter;
        if (!s_rCharacter)
        {
            continue;
        }

        auto s_eOriginalGlowType = Utils::GetProperty<ERenderGlowTypes>(s_rCharacter.m_ref, c_GlowTypePropertyName).value_or(ERenderGlowTypes::ERENDERGLOWTYPE_CIVILIAN);
        Utils::SetProperty<ERenderGlowTypes>(s_rCharacter.m_ref, c_GlowTypePropertyName, m_eGlowType);


        m_mOriginalGlowTypes[s_pActor] = s_eOriginalGlowType;
    }
}

void ZInstinctModifierEffectBase::Stop()
{
    for (auto& [s_pActor, s_eOriginalGlowType] : m_mOriginalGlowTypes)
    {
        if (!s_pActor)
        {
            continue;
        }

        auto s_rCharacter = s_pActor->m_rCharacter;
        if (!s_rCharacter)
        {
            continue;
        }

		Utils::SetProperty<ERenderGlowTypes>(s_rCharacter.m_ref, c_GlowTypePropertyName, s_eOriginalGlowType);
    }

    m_mOriginalGlowTypes.clear();
}

REGISTER_CHAOS_EFFECT(ZInstinctAllActorsTargetsEffect);
REGISTER_CHAOS_EFFECT(ZInstinctAllActorsCrowdEffect);
