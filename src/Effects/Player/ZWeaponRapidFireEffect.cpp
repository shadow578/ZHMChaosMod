#include "ZWeaponRapidFireEffect.h"

#include <Glacier/ZGameTime.h>

#include "Registry.h"
#include "Helpers/Utils.h"
#include "Helpers/PlayerUtils.h"

void ZWeaponRapidFireEffect::OnModInitialized()
{
    if (Hooks::ZHM5ItemWeapon_IsFiring)
    {
        Hooks::ZHM5ItemWeapon_IsFiring->AddDetour(this, &ZWeaponRapidFireEffect::ZHM5ItemWeapon_IsFiring);
    }
}

void ZWeaponRapidFireEffect::OnModUnload()
{
    if (Hooks::ZHM5ItemWeapon_IsFiring)
    {
        Hooks::ZHM5ItemWeapon_IsFiring->RemoveDetour(&ZWeaponRapidFireEffect::ZHM5ItemWeapon_IsFiring);
    }
}

bool ZWeaponRapidFireEffect::Available() const
{
    return IChaosEffect::Available() && Hooks::ZHM5ItemWeapon_IsFiring != nullptr;
}

bool ZWeaponRapidFireEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZWeaponRapidFireEffect>(p_pOtherEffect);
}

void ZWeaponRapidFireEffect::Start()
{
    m_bActive = true;
}

void ZWeaponRapidFireEffect::Stop()
{
    m_bActive = false;
}

DEFINE_BASE_CLASS_INSTANCES_HELPER(ZWeaponRapidFireEffect);

DEFINE_BASE_CLASS_DETOUR(
    ZWeaponRapidFireEffect,
    bool,
    ZHM5ItemWeapon_IsFiring,
    MAKE_FORWARD_PARAMS(th),
    IFirearm* th
)
{
    if (!m_bActive || !th)
    {
        return {HookAction::Continue()};
    }

    ZHM5ItemWeapon* s_pWeapon = static_cast<ZHM5ItemWeapon*>(th);
    if (!s_pWeapon || !s_pWeapon->m_pOwner)
    {
        return {HookAction::Continue()};
    }

    // rapid fire only applies to local player
    const auto s_rPlayer = Utils::GetLocalPlayer();
    if (s_pWeapon->m_pOwner == s_rPlayer.m_entityRef)
    {
        s_pWeapon->m_tLastShootTime = ZGameTime();
    }

    return {HookAction::Continue()};
}

REGISTER_CHAOS_EFFECT(ZWeaponRapidFireEffect);
