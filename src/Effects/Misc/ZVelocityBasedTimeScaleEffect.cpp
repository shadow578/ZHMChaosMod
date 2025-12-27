#include "ZVelocityBasedTimeScaleEffect.h"
#include "ZSlowTimeScaleEffect.h"

#include "Globals.h"
#include "IPluginInterface.h"

#include <Glacier/ZGameTime.h>
#include <Glacier/SGameUpdateEvent.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/ImGuiExtras.h"


ZVelocityBasedTimeScaleEffect::ZVelocityBasedTimeScaleEffect()
    : m_VelocityTracker(3), // low sample count for more responsiveness
    m_fVelocityToTimeScaleFactor(0.3f), // running speed ~= 1.0 time scale
    m_fPreviousTimeScale(1.0f)
{
}

void ZVelocityBasedTimeScaleEffect::Start()
{
    m_fPreviousTimeScale = Globals::GameTimeManager->m_fTimeMultiplier1;
    m_bActive = true;
}

void ZVelocityBasedTimeScaleEffect::Stop()
{
    Globals::GameTimeManager->m_fTimeMultiplier1 = m_fPreviousTimeScale;
    m_bActive = false;
}

void ZVelocityBasedTimeScaleEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!m_bActive)
    {
        return;
    }

    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player)
    {
        return;
    }

    m_VelocityTracker.Update(p_UpdateEvent, s_Player.m_ref);

    if (m_VelocityTracker.IsVelocityValid())
    {
        Globals::GameTimeManager->m_fTimeMultiplier1 = std::clamp(
            m_VelocityTracker.GetVelocity() * m_fVelocityToTimeScaleFactor, 
            0.05f, // not 0 since that would freeze the game completely
            1.0f
        );
    }
}

void ZVelocityBasedTimeScaleEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Time Scale: {:.2f}",
        Globals::GameTimeManager->m_fTimeMultiplier1
    ).c_str());

    ImGui::TextUnformatted(fmt::format("Velocity: {:.2f}{}",
        m_VelocityTracker.GetVelocity(),
        m_VelocityTracker.IsVelocityValid() ? "" : " (invalid)"
    ).c_str());

    ImGuiEx::DragFloat(
        "Velocity to Time Scale Factor",
        &m_fVelocityToTimeScaleFactor,
        0.0f,
        1.0f
    );
}

bool ZVelocityBasedTimeScaleEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
        // ZSlowTimeScaleEffect also modifies time scale, so not compatible
        && !Utils::IsInstanceOf<ZSlowTimeScaleEffect>(p_pOther);
}

REGISTER_CHAOS_EFFECT(ZVelocityBasedTimeScaleEffect);
