#include "ZRecoilModifierEffect.h"

#include <imgui.h>

#include <Glacier/ZHitman5.h>

#include "Registry.h"
#include "Helpers/Utils.h"

void ZRecoilModifierEffect::OnModInitialized()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->AddDetour(this, &ZRecoilModifierEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

void ZRecoilModifierEffect::OnModUnload()
{
    if (Hooks::ZHM5WeaponRecoilController_RecoilWeapon)
    {
        Hooks::ZHM5WeaponRecoilController_RecoilWeapon->RemoveDetour(&ZRecoilModifierEffect::ZHM5WeaponRecoilController_RecoilWeapon);
    }
}

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

DEFINE_BASE_CLASS_DETOUR(
    ZRecoilModifierEffect,
    void,
    ZHM5WeaponRecoilController_RecoilWeapon,
    MAKE_FORWARD_PARAMS(th, rWeapon),
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

REGISTER_CHAOS_EFFECT_PARAM(zero, ZRecoilModifierEffect, "zero", "No Recoil", SVector2(0.f, 0.f));
REGISTER_CHAOS_EFFECT_PARAM(extreme, ZRecoilModifierEffect, "extreme", "Extreme Recoil", SVector2(5.f, 5.f));
