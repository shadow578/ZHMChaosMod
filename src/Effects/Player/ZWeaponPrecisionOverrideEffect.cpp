#include "ZWeaponPrecisionOverrideEffect.h"

#include "Registry.h"
#include "Helpers/Utils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/ImGuiExtras.h"

void ZWeaponPrecisionOverrideEffect::OnModInitialized()
{
    if (Hooks::ZHM5ItemWeapon_FireProjectiles)
    {
        Hooks::ZHM5ItemWeapon_FireProjectiles->AddDetour(this, &ZWeaponPrecisionOverrideEffect::ZHM5ItemWeapon_FireProjectiles);
    }
}

void ZWeaponPrecisionOverrideEffect::OnModUnload()
{
    if (Hooks::ZHM5ItemWeapon_FireProjectiles)
    {
        Hooks::ZHM5ItemWeapon_FireProjectiles->RemoveDetour(&ZWeaponPrecisionOverrideEffect::ZHM5ItemWeapon_FireProjectiles);
    }
}

bool ZWeaponPrecisionOverrideEffect::Available() const
{
    return IChaosEffect::Available() && Hooks::ZHM5ItemWeapon_FireProjectiles != nullptr;
}

bool ZWeaponPrecisionOverrideEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZWeaponPrecisionOverrideEffect>(p_pOtherEffect);
}

void ZWeaponPrecisionOverrideEffect::OnDrawDebugUI()
{
    ImGuiEx::DragFloat("Precision Factor", &m_fPrecisionFactor, 0.f, 10.f);
}

void ZWeaponPrecisionOverrideEffect::Start()
{
    m_bActive = true;
}

void ZWeaponPrecisionOverrideEffect::Stop()
{
    m_bActive = false;
}

DEFINE_BASE_CLASS_DETOUR(
    ZWeaponPrecisionOverrideEffect,
    bool,
    ZHM5ItemWeapon_FireProjectiles,
    MAKE_FORWARD_PARAMS(th, p_bMayStartSound),
    ZHM5ItemWeapon* th,
    bool p_bMayStartSound
)
{
    if (!m_bActive || !th)
    {
        return {HookAction::Continue()};
    }

    // apply only for the local player
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (th->m_pOwner != s_rPlayer.m_entityRef)
    {
        return {HookAction::Continue()};
    }

    const auto s_fOriginalPrecisionFactor = th->m_fPrecisionFactor;
    th->m_fPrecisionFactor = m_fPrecisionFactor;

    const auto s_bResult = p_Hook->CallOriginal(th, p_bMayStartSound);

    th->m_fPrecisionFactor = s_fOriginalPrecisionFactor;

    return {HookAction::Return(), s_bResult};
}

REGISTER_CHAOS_EFFECT_PARAM(
    zero,
    ZWeaponPrecisionOverrideEffect,
    "zero",
    "Super Precision",
    0.f
);
REGISTER_CHAOS_EFFECT_PARAM(
    high,
    ZWeaponPrecisionOverrideEffect,
    "high",
    "Bad Aim",
    500.f // too high and the game just kinda gives up...
);
