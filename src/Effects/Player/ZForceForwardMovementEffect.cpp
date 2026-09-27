#include "ZForceForwardMovementEffect.h"

#include <Glacier/ZInput.h>

#include "Registry.h"
#include "Helpers/Utils.h"
#include "Helpers/InputActionNames.h"

#include "ZDisableInputsEffect.h"
#include "ZInvertControlsEffect.h"

void ZForceForwardMovementEffect::OnModInitialized()
{
    if (!Hooks::ZInputAction_Digital || !Hooks::ZInputAction_Analog)
    {
        return;
    }

    Hooks::ZInputAction_Analog->AddDetour(this, &ZForceForwardMovementEffect::OnInputActionAnalog);
}

void ZForceForwardMovementEffect::OnModUnload()
{
    if (!Hooks::ZInputAction_Digital || !Hooks::ZInputAction_Analog)
    {
        return;
    }

    Hooks::ZInputAction_Analog->RemoveDetour(&ZForceForwardMovementEffect::OnInputActionAnalog);
}

bool ZForceForwardMovementEffect::Available() const
{
    return IChaosEffect::Available() && Hooks::ZInputAction_Analog != nullptr;
}

bool ZForceForwardMovementEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZInvertControlsEffect>(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZDisableInputsEffect>(p_pOtherEffect);
}

void ZForceForwardMovementEffect::Start()
{
    m_bEnable = true;
}

void ZForceForwardMovementEffect::Stop()
{
    m_bEnable = false;
}

DEFINE_PLUGIN_DETOUR(ZForceForwardMovementEffect, float32, OnInputActionAnalog, ZInputAction* th, int a2)
{
    if (m_bEnable)
    {
        const std::string s_sName = th->m_szName;
        if (s_sName == InputActionNames::Keyboard::c_sVertical || s_sName == InputActionNames::Controller::c_sLeftStickVertical)
        {
            return {HookAction::Return(), 1.0f};
        }
    }

    return HookResult<float32>(HookAction::Continue());
}

REGISTER_CHAOS_EFFECT(ZForceForwardMovementEffect)
