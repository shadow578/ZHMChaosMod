#pragma once
#include "IChaosEffect.h"

#include <Glacier/Enums.h>

#include <map>

class ZInstinctModifierEffectBase : public IChaosEffect
{
public:
    ZInstinctModifierEffectBase(const ERenderGlowTypes p_eGlowType) 
        : m_eGlowType(p_eGlowType) 
    {
    }

    void Start() override;
    void Stop() override;

private:
    const ERenderGlowTypes m_eGlowType;

    std::unordered_map<ZActor*, ERenderGlowTypes> m_mOriginalGlowTypes;
};

class ZInstinctAllActorsTargetsEffect : public ZInstinctModifierEffectBase
{
public:
    ZInstinctAllActorsTargetsEffect()
        : ZInstinctModifierEffectBase(ERenderGlowTypes::ERENDERGLOWTYPE_CONTRACT_TARGET)
    {
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Social Anxiety Mode";
    }
};

class ZInstinctAllActorsCrowdEffect : public ZInstinctModifierEffectBase
{
public:
    ZInstinctAllActorsCrowdEffect()
        : ZInstinctModifierEffectBase(ERenderGlowTypes::ERENDERGLOWTYPE_NONE)
    {
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Guard-Level Instinct";
    }
};
