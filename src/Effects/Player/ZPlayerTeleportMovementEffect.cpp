#include "ZPlayerTeleportMovementEffect.h"

#include <Glacier/ZInput.h>
#include <Glacier/ZSpatialEntity.h>
#include <Glacier/ZCollision.h>
#include <Glacier/SGameUpdateEvent.h>

#include <Logging.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/Utils.h"
#include "Helpers/CameraUtils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/InputActionNames.h"
#include "Helpers/ImGuiExtras.h"

#include "ZDisableInputsEffect.h"
#include "ZForceForwardMovementEffect.h"

#define TAG "[ZPlayerTeleportMovementEffect] "

void ZPlayerTeleportMovementEffect::OnModInitialized()
{
    if (!Hooks::ZInputAction_Analog)
    {
        return;
    }

    Hooks::ZInputAction_Analog->AddDetour(this, &ZPlayerTeleportMovementEffect::OnInputActionAnalog);
}

void ZPlayerTeleportMovementEffect::OnModUnload()
{
    if (!Hooks::ZInputAction_Analog)
    {
        return;
    }

    Hooks::ZInputAction_Analog->RemoveDetour(&ZPlayerTeleportMovementEffect::OnInputActionAnalog);
}

bool ZPlayerTeleportMovementEffect::Available() const
{
    return IChaosEffect::Available()
           && Hooks::ZInputAction_Analog != nullptr
           && *Globals::CollisionManager != nullptr;
}

void ZPlayerTeleportMovementEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("State: {}", StateToName(m_eState)).c_str());
    if (m_eState == EState::COOLDOWN)
    {
        ImGui::TextUnformatted(fmt::format("Cooldown Remaining: {:.2f}s", m_fCooldownRemaining).c_str());
    }

    ImGuiEx::DragFloat("Raycast Start Offset", &m_fTeleportRaycastStartOffset, 0.0f, 5.0f);
}

void ZPlayerTeleportMovementEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_fCooldownTime = p_pConfiguration->GetFloat("Cooldown", m_fCooldownTime);
    m_fMaxTeleportDistance = p_pConfiguration->GetFloat("MaxDistance", m_fMaxTeleportDistance);
}

void ZPlayerTeleportMovementEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGuiEx::DragFloat("Cooldown Time", &m_fCooldownTime, 0.5f, 30.0f, "%.2f s"))
    {
        p_pConfiguration->SetDouble("Cooldown", m_fCooldownTime);
    }

    if (ImGuiEx::DragFloat("Max Teleport Distance", &m_fMaxTeleportDistance, 10.0f, 500.0f))
    {
        p_pConfiguration->SetDouble("MaxDistance", m_fMaxTeleportDistance);
    }
}

bool ZPlayerTeleportMovementEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZDisableInputsEffect>(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZForceForwardMovementEffect>(p_pOtherEffect);
}

void ZPlayerTeleportMovementEffect::Start()
{
    m_eState = EState::WAIT_FOR_INPUT;
}

void ZPlayerTeleportMovementEffect::Stop()
{
    m_eState = EState::DISABLED;
}

void ZPlayerTeleportMovementEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    switch (m_eState)
    {
    case EState::TELEPORT_NEXT_FRAME: {
        const auto s_eResult = PerformTeleport();

        // if teleported (or for some reason failed to teleport, tho that should be unlikely), go to cooldown.
        // in case we simply didn't hit anything, skip the cooldown and go directly to waiting for input.
        if (s_eResult != ETeleportResult::NO_HIT)
        {
            m_eState = EState::COOLDOWN;
            m_fCooldownRemaining = m_fCooldownTime;
        }
        else // ETeleportResult::NO_HIT
        {
            m_eState = EState::WAIT_FOR_INPUT;
        }
        break;
    }
    case EState::COOLDOWN: {
        m_fCooldownRemaining -= static_cast<float32>(p_UpdateEvent.m_GameTimeDelta.ToSeconds());
        if (m_fCooldownRemaining <= 0.0f)
        {
            m_eState = EState::WAIT_FOR_INPUT;
        }
        break;
    }
    default:
        break;
    }
}

ZPlayerTeleportMovementEffect::ETeleportResult ZPlayerTeleportMovementEffect::PerformTeleport()
{
    // get main camera position and orientation
    ZEntityRef s_rCamera;
    if (!Utils::GetActiveCamera(s_rCamera))
    {
        Logger::Error(TAG "Failed to get active camera");
        return ETeleportResult::FAILURE;
    }

    const auto s_rCameraSpatial = TEntityRef<ZSpatialEntity>(s_rCamera);
    if (!s_rCameraSpatial)
    {
        Logger::Error(TAG "Failed to get camera spatial");
        return ETeleportResult::FAILURE;
    }

    const auto s_mCameraTransform = s_rCameraSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    const auto s_vCameraForward = -s_mCameraTransform.CameraBackward;

    // perform a raycast in the forward direction to find teleport location
    if (!*Globals::CollisionManager)
    {
        Logger::Error(TAG "CollisionManager not available");
        return ETeleportResult::FAILURE;
    }

    ZRayQueryInput s_RayInput{
        // start at a slight offset to avoid colliding with wall right behind the camera
        .m_vFrom = s_mCameraTransform.Trans + (s_vCameraForward * m_fTeleportRaycastStartOffset),
        .m_vTo = s_mCameraTransform.Trans + (s_vCameraForward * m_fMaxTeleportDistance),
    };
    ZRayQueryOutput s_RayOutput{};

    if (!(*Globals::CollisionManager)->RayCastClosestHit(s_RayInput, &s_RayOutput))
    {
        Logger::Warn(TAG "Raycast did not hit anything!");
        return ETeleportResult::NO_HIT;
    }

    const auto s_vPosition = s_RayOutput.m_vPosition;

    // teleport player to hit position, preserve rotation
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (!s_rPlayer)
    {
        Logger::Error(TAG "Failed to get local player");
        return ETeleportResult::FAILURE;
    }

    const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef);
    if (!s_rPlayerSpatial)
    {
        Logger::Error(TAG "Failed to get player spatial");
        return ETeleportResult::FAILURE;
    }

    auto s_mPlayerTransform = s_rPlayerSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    s_mPlayerTransform.Pos = s_vPosition;
    s_rPlayerSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mPlayerTransform);

    Logger::Debug(TAG "Performed teleport successfully!");
    return ETeleportResult::SUCCESS;
}

DEFINE_PLUGIN_DETOUR(ZPlayerTeleportMovementEffect, double, OnInputActionAnalog, ZInputAction* th, int a2)
{
    if (m_eState != EState::DISABLED)
    {
        const std::string s_sName = th->m_szName;

        const bool s_bIsVerticalMovement = s_sName == InputActionNames::Keyboard::c_sVertical || s_sName == InputActionNames::Controller::c_sLeftStickVertical;
        const bool s_bIsHorizontalMovement = s_sName == InputActionNames::Keyboard::c_sHorizontal || s_sName == InputActionNames::Controller::c_sLeftStickHorizontal;
        if (s_bIsVerticalMovement || s_bIsHorizontalMovement)
        {
            auto s_fValueRaw = p_Hook->CallOriginal(th, a2);

            // note: hook uses float32, but typed as float64.
            // so we need to unpack and repack to/from float32.
            auto s_fValue = *Utils::CastRaw<float32>(&s_fValueRaw);

            // for positive vertical movement (= walk forward), trigger teleport
            // only if above threshold to prevent accidental activation (only relevant for controllers!)
            if (m_eState == EState::WAIT_FOR_INPUT && s_bIsVerticalMovement && s_fValue > 0.3f)
            {
                m_eState = EState::TELEPORT_NEXT_FRAME;
            }

            // disable all movement via inputs while active
            s_fValue = 0.0f;

            auto s_fValueReturn = *Utils::CastRaw<float64>(&s_fValue);
            return {HookAction::Return(), s_fValueReturn};
        }
    }

    return HookResult<float64>(HookAction::Continue());
}

std::string_view ZPlayerTeleportMovementEffect::StateToName(const EState p_eState)
{
    switch (p_eState)
    {
    case EState::DISABLED:
        return "DISABLED";
    case EState::WAIT_FOR_INPUT:
        return "WAIT_FOR_INPUT";
    case EState::TELEPORT_NEXT_FRAME:
        return "TELEPORT_NEXT_FRAME";
    case EState::COOLDOWN:
        return "COOLDOWN";
    default:
        return "UNKNOWN";
    }
}

REGISTER_CHAOS_EFFECT(ZPlayerTeleportMovementEffect)
