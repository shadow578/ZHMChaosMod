#include "ZForceForwardMovementEffect.h"

#include <Glacier/ZInput.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/InputActionNames.h"

#include "ZDisableInputsEffect.h"
#include "ZInvertControlsEffect.h"

void ZForceForwardMovementEffect::OnModInitialized()
{
    Hooks::ZInputAction_Analog->AddDetour(this, &ZForceForwardMovementEffect::OnInputActionAnalog);
}

void ZForceForwardMovementEffect::OnModUnload()
{
    Hooks::ZInputAction_Analog->RemoveDetour(&ZForceForwardMovementEffect::OnInputActionAnalog);
}

bool ZForceForwardMovementEffect::Available()
{
    return IChaosEffect::Available() &&
        Hooks::ZInputAction_Analog != nullptr;
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

DEFINE_PLUGIN_DETOUR(ZForceForwardMovementEffect, double, OnInputActionAnalog, ZInputAction* th, int a2)
{
    if (m_bEnable)
    {
        const std::string s_sName = th->m_szName;
        if (s_sName == InputActionNames::Keyboard::c_sVertical ||
            s_sName == InputActionNames::Controller::c_sLeftStickVertical)
        {
            // note: hook uses float32, but typed as float64.
            // so we need to unpack and repack to/from float32.
            auto s_fValue = 1.0f;

            auto s_fValueReturn = *Utils::CastRaw<float64>(&s_fValue);
            return { HookAction::Return(), s_fValueReturn };
        }
    }

    return HookResult<float64>(HookAction::Continue());
}

REGISTER_CHAOS_EFFECT(ZForceForwardMovementEffect)
