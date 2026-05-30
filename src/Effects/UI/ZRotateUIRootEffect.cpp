#include "ZRotateUIRootEffect.h"

#include <imgui.h>
#include <Logging.h>

#include <cmath>

#include <Glacier/ZGameLoopManager.h>
#include <Glacier/SGameUpdateEvent.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"

#include "Entity/EntityIds.h"
#include "Entity/Bindings/SZUIControlEntityBinding.h"

#include "Effects/UI/ZNoHUDEffect.h"

#define TAG "[ZRotateUIRootEffect] "

constexpr float32 c_fPI = 3.14159f;

void ZRotateUIRootEffect::OnEngineInitialized()
{
    const ZMemberDelegate<ZRotateUIRootEffect, void(const SGameUpdateEvent&)> s_Delegate(this, &ZRotateUIRootEffect::OnFrameUpdateAlways);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdateAlways);
}

void ZRotateUIRootEffect::OnModUnload()
{
    m_bActive = false;

    const ZMemberDelegate<ZRotateUIRootEffect, void(const SGameUpdateEvent&)> s_Delegate(this, &ZRotateUIRootEffect::OnFrameUpdateAlways);
    Globals::GameLoopManager->UnregisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdateAlways);
}

void ZRotateUIRootEffect::OnEnterScene()
{
    m_rUIRoot = Utils::ZEntityFinder()
                    .EntityID(EntityId::HM3::GameEssentials::MasterRoot)
                    .FindFirst();
    m_bActive = false;
}

void ZRotateUIRootEffect::OnClearScene()
{
    m_rUIRoot = {};
    m_bActive = false;
}

bool ZRotateUIRootEffect::Available() const
{
    return IChaosEffect::Available()
           && m_rUIRoot;
}

void ZRotateUIRootEffect::OnDrawDebugUI()
{
    ImGui::DragFloat3("Rotation Rate", &m_vRotationRate.x);
    ImGui::TextUnformatted(fmt::format("Elapsed Time: {:.2f}s", m_fTimeElapsed).c_str());
}

void ZRotateUIRootEffect::Start()
{
    m_bActive = true;
    m_fTimeElapsed = 0.0f;
}

void ZRotateUIRootEffect::Stop()
{
    m_bActive = false;

    auto s_UIBinding = SZUIControlEntityBinding(m_rUIRoot);
    s_UIBinding.m_vRotation = SVector3(0.f, 0.f, 0.f);
}

void ZRotateUIRootEffect::OnFrameUpdateAlways(const SGameUpdateEvent& p_UpdateEvent)
{
    if (!m_bActive || !m_rUIRoot)
    {
        return;
    }

    m_fTimeElapsed += static_cast<float32>(p_UpdateEvent.m_RealTimeDelta.ToSeconds());

    const auto s_vRotation = SVector3(
        90.f * std::sin(m_vRotationRate.x * m_fTimeElapsed),
        90.f * std::sin(m_vRotationRate.y * m_fTimeElapsed),
        90.f * std::sin(m_vRotationRate.z * m_fTimeElapsed)
    );

    auto s_UIBinding = SZUIControlEntityBinding(m_rUIRoot);
    s_UIBinding.m_vRotation = s_vRotation;
}

REGISTER_CHAOS_EFFECT_PARAM(swing, ZRotateUIRootEffect, "swing", "HUD Swing", SVector3(1.f, 0.f, 0.f));
REGISTER_CHAOS_EFFECT_PARAM(chaotic, ZRotateUIRootEffect, "chaotic", "UI go Wheee!", SVector3(1.f, 0.8f, 0.f));
