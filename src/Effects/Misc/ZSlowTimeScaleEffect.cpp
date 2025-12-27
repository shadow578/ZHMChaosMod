#include "ZSlowTimeScaleEffect.h"

#include "Globals.h"

#include "Glacier/ZGameTime.h"

#include "EffectRegistry.h"

void ZSlowTimeScaleEffect::Start()
{
    m_fPreviousTimeScale = Globals::GameTimeManager->m_fTimeMultiplier1;
    Globals::GameTimeManager->m_fTimeMultiplier1 = m_fTargetTimeScale;
}

void ZSlowTimeScaleEffect::Stop()
{
    Globals::GameTimeManager->m_fTimeMultiplier1 = m_fPreviousTimeScale;
}

std::string ZSlowTimeScaleEffect::GetName()
{
    return IChaosEffect::GetName() + "_" + std::to_string(static_cast<int>(m_fTargetTimeScale*100));
}

std::string ZSlowTimeScaleEffect::GetDisplayName()
{
    return fmt::format("{:.1f}x Game Speed", m_fTargetTimeScale);
}

IChaosEffect::EDuration ZSlowTimeScaleEffect::GetDuration()
{
    // slower than normal -> short duration
    return (m_fTargetTimeScale < 1.0f) ? EDuration::Short : EDuration::Full;
}

REGISTER_CHAOS_EFFECT_PARAM(SP02x, ZSlowTimeScaleEffect, 0.2f);
REGISTER_CHAOS_EFFECT_PARAM(SP5x, ZSlowTimeScaleEffect, 5.0f);

