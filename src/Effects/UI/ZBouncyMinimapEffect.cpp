#include "ZBouncyMinimapEffect.h"

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
#include "Effects/UI/ZLargeMinimapEffect.h"

#define TAG "[ZBouncyMinimapEffect] "

void ZBouncyMinimapEffect::OnEnterScene()
{
    m_rMinimapContainer = Utils::ZEntityFinder()
                              .EntityID(EntityId::HM3::Minimap::Container)
                              .FindFirst();
    m_bActive = false;
}

void ZBouncyMinimapEffect::OnClearScene()
{
    m_rMinimapContainer = {};
    m_bActive = false;
}

bool ZBouncyMinimapEffect::Available() const
{
    return IChaosEffect::Available()
           && m_rMinimapContainer;
}

bool ZBouncyMinimapEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           // joke doesn't really work when HUD is not visible...
           && !Utils::IsInstanceOf<ZNoHUDEffect>(p_pOther)

           // math doesn't work out with this one active
           && !Utils::IsInstanceOf<ZLargeMinimapEffect>(p_pOther);
}

void ZBouncyMinimapEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Position: {}, {}", m_vPosition.x, m_vPosition.y).c_str());
    ImGui::TextUnformatted(fmt::format("Velocity: {}, {}", m_vVelocity.x, m_vVelocity.y).c_str());

    ImGui::DragFloat("Velocity Multiplier", &m_fVelocityMultiplier, 1.f);

    ImGui::DragFloat2("Bounds Min", &m_vBoundsMin.x, .1f);
    ImGui::DragFloat2("Bounds Max", &m_vBoundsMax.x, .1f);
}

void ZBouncyMinimapEffect::Start()
{
    auto s_ContainerBinding = SZUIControlEntityBinding(m_rMinimapContainer);
    if (!s_ContainerBinding)
    {
        return;
    }

    const auto s_vPos = s_ContainerBinding.m_vPositionOffset;
    if (!s_vPos.has_value())
    {
        return;
    }

    m_vPosition = SVector2(s_vPos.value().x, s_vPos.value().y);
    m_vVelocity = SVector2(
        Math::GetRandomNumber<float32>(-15.f, 15.f),
        Math::GetRandomNumber<float32>(-15.f, 15.f)
    );
    m_bActive = true;
}

void ZBouncyMinimapEffect::Stop()
{
    m_bActive = false;

    auto s_ContainerBinding = SZUIControlEntityBinding(m_rMinimapContainer);
    s_ContainerBinding.m_vPositionOffset = SVector3(419.f, -16.f, 0.f); // default from minimap TEMP
}

void ZBouncyMinimapEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!m_bActive || !m_rMinimapContainer)
    {
        return;
    }

    auto s_ContainerBinding = SZUIControlEntityBinding(m_rMinimapContainer);
    if (!s_ContainerBinding)
    {
        return;
    }

    // apply velocity
    const auto s_fDeltaTime = static_cast<float32>(p_UpdateEvent.m_GameTimeDelta.ToSeconds());
    m_vPosition.x += m_vVelocity.x * m_fVelocityMultiplier * s_fDeltaTime;
    m_vPosition.y += m_vVelocity.y * m_fVelocityMultiplier * s_fDeltaTime;

    // when hitting bounds, limit position and apply randomized (inverse) bouncy velocity
    if (m_vPosition.x < m_vBoundsMin.x)
    {
        m_vPosition.x = m_vBoundsMin.x;
        m_vVelocity.x *= Math::GetRandomNumber<float32>(-1.2f, -0.8f);
    }
    if (m_vPosition.x > m_vBoundsMax.x)
    {
        m_vPosition.x = m_vBoundsMax.x;
        m_vVelocity.x *= Math::GetRandomNumber<float32>(-1.2f, -0.8f);
    }
    if (m_vPosition.y < m_vBoundsMin.y)
    {
        m_vPosition.y = m_vBoundsMin.y;
        m_vVelocity.y *= Math::GetRandomNumber<float32>(-1.2f, -0.8f);
    }
    if (m_vPosition.y > m_vBoundsMax.y)
    {
        m_vPosition.y = m_vBoundsMax.y;
        m_vVelocity.y *= Math::GetRandomNumber<float32>(-1.2f, -0.8f);
    }

    // set position ignoring z
    s_ContainerBinding.m_vPositionOffset = SVector3(m_vPosition.x, m_vPosition.y, 0.f);
}

REGISTER_CHAOS_EFFECT(ZBouncyMinimapEffect);
