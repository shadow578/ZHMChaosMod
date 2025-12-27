#include "ZDisableInputsEffect.h";

#include <Glacier/ZInput.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/InputActionNames.h"

#include "ZInvertControlsEffect.h"
#include "ZForceForwardMovementEffect.h"

void ZDisableInputsEffect::OnModInitialized()
{
    Hooks::ZInputAction_Digital->AddDetour(this, &ZDisableInputsEffect::OnInputActionDigital);
    Hooks::ZInputAction_Analog->AddDetour(this, &ZDisableInputsEffect::OnInputActionAnalog);
}

void ZDisableInputsEffect::OnModUnload()
{
    Hooks::ZInputAction_Digital->RemoveDetour(&ZDisableInputsEffect::OnInputActionDigital);
    Hooks::ZInputAction_Analog->RemoveDetour(&ZDisableInputsEffect::OnInputActionAnalog);
}

bool ZDisableInputsEffect::Available()
{
    return IChaosEffect::Available() &&
        Hooks::ZInputAction_Digital != nullptr &&
        Hooks::ZInputAction_Analog != nullptr;
}

bool ZDisableInputsEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
        && !Utils::IsInstanceOf<ZInvertControlsEffect>(p_pOtherEffect)
        && !Utils::IsInstanceOf<ZForceForwardMovementEffect>(p_pOtherEffect);
}

void ZDisableInputsEffect::Start()
{
    m_bEnable = true;
}

void ZDisableInputsEffect::Stop()
{
    m_bEnable = false;
}

DEFINE_PLUGIN_DETOUR(ZDisableInputsEffect, bool, OnInputActionDigital, ZInputAction* th, int a2)
{
    if (m_bEnable)
    {
        // allow Start key so player can pause / unpause
        const std::string s_sName = th->m_szName;
        if (s_sName != InputActionNames::Keyboard::c_sStart &&
            s_sName != InputActionNames::Controller::c_sStart)
        {
            return { HookAction::Return(), false };
        }
    }

    return HookResult<bool>(HookAction::Continue());
}

DEFINE_PLUGIN_DETOUR(ZDisableInputsEffect, double, OnInputActionAnalog, ZInputAction* th, int a2)
{
    if (m_bEnable)
    {
        // note: hook uses float32, but typed as float64.
        // so we need to unpack and repack to/from float32.
        auto s_fValue = 0.0f;
        auto s_fValueReturn = *Utils::CastRaw<float64>(&s_fValue);
        return { HookAction::Return(), s_fValueReturn };
    }

    return HookResult<float64>(HookAction::Continue());
}

REGISTER_CHAOS_EFFECT(ZDisableInputsEffect)
