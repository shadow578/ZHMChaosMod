#include "ZInstinctModifierEffect.h"

#include <Glacier/ZActor.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"

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

        auto s_eOriginalGlowType = s_rCharacter.m_ref.GetProperty<ERenderGlowTypes>(c_GlowTypePropertyName).Get();
        s_rCharacter.m_ref.SetProperty(c_GlowTypePropertyName, m_eGlowType);

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

        s_rCharacter.m_ref.SetProperty(c_GlowTypePropertyName, s_eOriginalGlowType);
    }

    m_mOriginalGlowTypes.clear();
}

REGISTER_CHAOS_EFFECT(ZInstinctAllActorsTargetsEffect);
REGISTER_CHAOS_EFFECT(ZInstinctAllActorsCrowdEffect);
