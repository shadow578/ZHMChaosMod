#include "RecoilModifierEffects.h"

#include <imgui.h>

#include <Glacier/ZHitman5.h>

#include "Registry.h"
#include "Helpers/Utils.h"

bool ZRecoilModifierEffect::Available() const
{
    return IChaosEffect::Available() && Hooks::ZHM5WeaponRecoilController_RecoilWeapon != nullptr;
}

bool ZRecoilModifierEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZRecoilModifierEffect>(p_pOtherEffect);
}

void ZRecoilModifierEffect::OnDrawDebugUI()
{
    ImGui::DragFloat2("Recoil Multiplier", &m_vRecoilMultiplier.x);
}

void ZRecoilModifierEffect::Start()
{
    m_bActive = true;
}

void ZRecoilModifierEffect::Stop()
{
    m_bActive = false;
}

// ZNoRecoilEffect
void ZNoRecoilEffect::OnModInitialized()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->AddDetour(this, &ZNoRecoilEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

void ZNoRecoilEffect::OnModUnload()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->RemoveDetour(&ZNoRecoilEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

DEFINE_PLUGIN_DETOUR(
    ZNoRecoilEffect,
    void,
    ZHM5WeaponRecoilController_RecoilWeapon,
    ZHM5WeaponRecoilController* th,
    const TEntityRef<ZHM5ItemWeapon>& rWeapon
)
{
    if (!m_bActive)
    {
        return {HookAction::Continue()};
    }

    p_Hook->CallOriginal(th, rWeapon);

    th->m_vRecoil.x *= m_vRecoilMultiplier.x;
    th->m_vRecoil.y *= m_vRecoilMultiplier.y;

    return {HookAction::Return()};
}

REGISTER_CHAOS_EFFECT(ZNoRecoilEffect);

// ZExtremeRecoilEffect
void ZExtremeRecoilEffect::OnModInitialized()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->AddDetour(this, &ZExtremeRecoilEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

void ZExtremeRecoilEffect::OnModUnload()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->RemoveDetour(&ZExtremeRecoilEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

DEFINE_PLUGIN_DETOUR(
    ZExtremeRecoilEffect,
    void,
    ZHM5WeaponRecoilController_RecoilWeapon,
    ZHM5WeaponRecoilController* th,
    const TEntityRef<ZHM5ItemWeapon>& rWeapon
)
{
    if (!m_bActive)
    {
        return {HookAction::Continue()};
    }

    p_Hook->CallOriginal(th, rWeapon);

    th->m_vRecoil.x *= m_vRecoilMultiplier.x;
    th->m_vRecoil.y *= m_vRecoilMultiplier.y;

    return {HookAction::Return()};
}
REGISTER_CHAOS_EFFECT(ZExtremeRecoilEffect);
