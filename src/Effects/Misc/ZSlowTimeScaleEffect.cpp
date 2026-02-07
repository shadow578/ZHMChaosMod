#include "ZSlowTimeScaleEffect.h"

#include "Globals.h"

#include "Glacier/ZGameTime.h"

#include "EffectRegistry.h"

void ZSlowTimeScaleEffect::Start()
{
    // use m_fDebugTimeMultiplier instead of m_fGameTimeMultiplier, since GameTimeMultiplier is used by the game
    // for e.g. instinct, and that would interfere with the effect.
    m_fPreviousTimeScale = Globals::GameTimeManager->m_fDebugTimeMultiplier;
    Globals::GameTimeManager->m_fDebugTimeMultiplier = m_fTargetTimeScale;
}

void ZSlowTimeScaleEffect::Stop()
{
    Globals::GameTimeManager->m_fDebugTimeMultiplier = m_fPreviousTimeScale;
}

std::string ZSlowTimeScaleEffect::GetName() const
{
    return IChaosEffect::GetName() + "_" + std::to_string(static_cast<int>(m_fTargetTimeScale*100));
}

std::string ZSlowTimeScaleEffect::GetDisplayName(const bool p_bVoting) const
{
    return fmt::format("{:.1f}x Game Speed", m_fTargetTimeScale);
}

IChaosEffect::EDuration ZSlowTimeScaleEffect::GetDuration() const
{
    // slower than normal -> short duration
    return (m_fTargetTimeScale < 1.0f) ? EDuration::Short : EDuration::Full;
}

REGISTER_CHAOS_EFFECT_PARAM(SP02x, ZSlowTimeScaleEffect, 0.2f);
REGISTER_CHAOS_EFFECT_PARAM(SP5x, ZSlowTimeScaleEffect, 5.0f);

