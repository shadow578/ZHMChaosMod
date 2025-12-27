#include "ZInvertControlsEffect.h"

#include <Glacier/ZInput.h>

#include "EffectRegistry.h"
#include "Helpers/Utils.h"
#include "Helpers/InputActionNames.h"

#include "ZDisableInputsEffect.h"
#include "ZForceForwardMovementEffect.h"

void ZInvertControlsEffect::OnModInitialized()
{
    Hooks::ZInputAction_Analog->AddDetour(this, &ZInvertControlsEffect::OnInputActionAnalog);
}

void ZInvertControlsEffect::OnModUnload()
{
    Hooks::ZInputAction_Analog->RemoveDetour(&ZInvertControlsEffect::OnInputActionAnalog);
}

bool ZInvertControlsEffect::Available() const
{
    return IChaosEffect::Available() &&
        Hooks::ZInputAction_Analog != nullptr;
}

bool ZInvertControlsEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
        && !Utils::IsInstanceOf<ZDisableInputsEffect>(p_pOtherEffect)
        && !Utils::IsInstanceOf<ZForceForwardMovementEffect>(p_pOtherEffect);
}

void ZInvertControlsEffect::Start()
{
    m_bEnable = true;
}

void ZInvertControlsEffect::Stop()
{
    m_bEnable = false;
}

DEFINE_PLUGIN_DETOUR(ZInvertControlsEffect, double, OnInputActionAnalog, ZInputAction* th, int a2)
{
    if (m_bEnable)
    {
        const std::string s_sName = th->m_szName;
        if (s_sName == InputActionNames::Keyboard::c_sVertical ||
            s_sName == InputActionNames::Keyboard::c_sHorizontal ||
            s_sName == InputActionNames::Keyboard::c_sLookVertical ||
            s_sName == InputActionNames::Keyboard::c_sLookHorizontal ||
            s_sName == InputActionNames::Controller::c_sLeftStickVertical ||
            s_sName == InputActionNames::Controller::c_sLeftStickHorizontal ||
            s_sName == InputActionNames::Controller::c_sRightStickVertical ||
            s_sName == InputActionNames::Controller::c_sRightStickHorizontal)
        {
            auto s_fValueRaw = p_Hook->CallOriginal(th, a2);

            // note: hook uses float32, but typed as float64.
            // so we need to unpack and repack to/from float32.
            auto s_fValue = *Utils::CastRaw<float32>(&s_fValueRaw);

            s_fValue *= -1.0f;

            auto s_fValueReturn = *Utils::CastRaw<float64>(&s_fValue);
            return { HookAction::Return(), s_fValueReturn };
        }
    }

    return HookResult<float64>(HookAction::Continue());
}

REGISTER_CHAOS_EFFECT(ZInvertControlsEffect)
