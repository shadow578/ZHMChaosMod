#include "ZShakyReticleEffect.h"

#include <imgui.h>
#include <Logging.h>

#include <Glacier/SGameUpdateEvent.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/Math.h"
#include "Helpers/Utils.h"
#include "Helpers/EntityUtils.h"

#include "Entity/EntityIds.h"
#include "Entity/Bindings/SZUIControlEntityBinding.h"

#include "Effects/UI/ZNoHUDEffect.h"

#define TAG "[ZShakyReticleEffect] "

void ZShakyReticleEffect::OnEnterScene()
{
    m_rHUDReticle = Utils::ZEntityFinder()
                        .EntityID(EntityId::HM3::GameEssentials::HUDReticle)
                        .FindFirst();
    m_bActive = false;
}

void ZShakyReticleEffect::OnClearScene()
{
    m_rHUDReticle = {};
    m_bActive = false;
}

bool ZShakyReticleEffect::Available() const
{
    return IChaosEffect::Available()
           && m_rHUDReticle;
}

bool ZShakyReticleEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           // joke doesn't really work when HUD is not visible...
           && !Utils::IsInstanceOf<ZNoHUDEffect>(p_pOther);
}

void ZShakyReticleEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Position: {}, {}", m_vPosition.x, m_vPosition.y).c_str());

    ImGui::DragFloat("Amplitude", &m_fShakeAmplitude, .1f);
    ImGui::DragFloat("Max Deflection", &m_fMaxDeflection, .5f);
}

void ZShakyReticleEffect::Start()
{
    m_vPosition = SVector2(0.f, 0.f);
    m_bActive = true;
}

void ZShakyReticleEffect::Stop()
{
    m_bActive = false;

    auto s_Binding = SZUIControlEntityBinding(m_rHUDReticle);
    s_Binding.m_vPositionOffset = SVector3(0.f, 0.f, 0.f); // default from gameessentials TEMP
}

void ZShakyReticleEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!m_bActive || !m_rHUDReticle)
    {
        return;
    }

    auto s_Binding = SZUIControlEntityBinding(m_rHUDReticle);
    if (!s_Binding)
    {
        return;
    }

    // add random deflection
    m_vPosition.x += Math::GetRandomNumber<float32>(-m_fShakeAmplitude, m_fShakeAmplitude);
    m_vPosition.y += Math::GetRandomNumber<float32>(-m_fShakeAmplitude, m_fShakeAmplitude);

    // limit deflection
    m_vPosition.x = Math::Clamp(m_vPosition.x, -m_fMaxDeflection, m_fMaxDeflection);
    m_vPosition.y = Math::Clamp(m_vPosition.y, -m_fMaxDeflection, m_fMaxDeflection);

    // set position ignoring z
    s_Binding.m_vPositionOffset = SVector3(m_vPosition.x, m_vPosition.y, 0.f);
}

REGISTER_CHAOS_EFFECT(ZShakyReticleEffect);
