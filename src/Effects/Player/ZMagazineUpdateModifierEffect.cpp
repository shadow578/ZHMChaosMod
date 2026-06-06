#include "ZMagazineUpdateModifierEffect.h"

#include "Registry.h"
#include "Helpers/Utils.h"
#include "Helpers/PlayerUtils.h"

void ZMagazineUpdateModifierEffect::OnModInitialized()
{
    if (Hooks::ZHM5ItemWeapon_SetBulletsInMagazine)
    {
        Hooks::ZHM5ItemWeapon_SetBulletsInMagazine->AddDetour(this, &ZMagazineUpdateModifierEffect::ZHM5ItemWeapon_SetBulletsInMagazine);
    }
}

void ZMagazineUpdateModifierEffect::OnModUnload()
{
    if (Hooks::ZHM5ItemWeapon_SetBulletsInMagazine)
    {
        Hooks::ZHM5ItemWeapon_SetBulletsInMagazine->RemoveDetour(&ZMagazineUpdateModifierEffect::ZHM5ItemWeapon_SetBulletsInMagazine);
    }
}

bool ZMagazineUpdateModifierEffect::Available() const
{
    return IChaosEffect::Available() && Hooks::ZHM5ItemWeapon_SetBulletsInMagazine != nullptr;
}

bool ZMagazineUpdateModifierEffect::IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const
{
    return IChaosEffect::IsCompatibleWith(p_pOtherEffect)
           && !Utils::IsInstanceOf<ZMagazineUpdateModifierEffect>(p_pOtherEffect);
}

void ZMagazineUpdateModifierEffect::OnDrawDebugUI()
{
}

void ZMagazineUpdateModifierEffect::Start()
{
    m_bActive = true;
}

void ZMagazineUpdateModifierEffect::Stop()
{
    m_bActive = false;
}

DEFINE_BASE_CLASS_INSTANCES_HELPER(ZMagazineUpdateModifierEffect);

DEFINE_BASE_CLASS_DETOUR(
    ZMagazineUpdateModifierEffect,
    void,
    ZHM5ItemWeapon_SetBulletsInMagazine,
    MAKE_FORWARD_PARAMS(th, p_nBullets),
    IFirearm* th,
    int32_t p_nBullets
)
{
    if (!m_bActive)
    {
        return {HookAction::Continue()};
    }

    ZHM5ItemWeapon* s_pWeapon = static_cast<ZHM5ItemWeapon*>(th);
    if (!s_pWeapon || !s_pWeapon->m_pOwner)
    {
        return {HookAction::Continue()};
    }

    // check if we should apply this effect based on weapon owner
    // only needed if not applied to all
    if (m_eApplyTo != EApplyTo::All)
    {
        // Note: GetLocalPlayer *may* technically return an empty ref, but in practice this should not happen
        // all following code is written such that even an empty ref does not cause crashes though
        const auto s_rPlayer = Utils::GetLocalPlayer();
        const auto s_bOwnedByPlayer = s_pWeapon->m_pOwner == s_rPlayer.m_entityRef;

        if (s_bOwnedByPlayer && m_eApplyTo == EApplyTo::Actor)
        {
            return {HookAction::Continue()};
        }
    }

    // effect should apply to weapon holder, do the thing
    switch (m_eKind)
    {
    case EKind::UnlimitedMags:
        p_nBullets = s_pWeapon->GetMagazineCapacity();
        s_pWeapon->m_nBulletsFired = 0;
        break;
    case EKind::EmptyMags:
        p_nBullets = 0;
        s_pWeapon->m_nBulletsFired = s_pWeapon->GetMagazineCapacity();
        break;
    case EKind::SingleBulletMags:
        if (p_nBullets > 1)
        {
            p_nBullets = 1;
            s_pWeapon->m_nBulletsFired = std::max(0, s_pWeapon->GetMagazineCapacity() - 1);
        }
        break;
    }

    p_Hook->CallOriginal(th, p_nBullets);
    return {HookAction::Return()};
}

REGISTER_CHAOS_EFFECT_PARAM(
    UnlimitedMags,
    ZMagazineUpdateModifierEffect,
    "UnlimitedMags",
    "Unlimited Ammo",
    ZMagazineUpdateModifierEffect::EApplyTo::Player,
    ZMagazineUpdateModifierEffect::EKind::UnlimitedMags
);
REGISTER_CHAOS_EFFECT_PARAM(
    EmptyMagsPlayer,
    ZMagazineUpdateModifierEffect,
    "EmptyMagsPlayer",
    "Oops, No Bullets!",
    ZMagazineUpdateModifierEffect::EApplyTo::Player,
    ZMagazineUpdateModifierEffect::EKind::EmptyMags
);
REGISTER_CHAOS_EFFECT_PARAM(
    EmptyMagsActors,
    ZMagazineUpdateModifierEffect,
    "EmptyMagsActors",
    "Security Budget Cuts",
    ZMagazineUpdateModifierEffect::EApplyTo::Actor,
    ZMagazineUpdateModifierEffect::EKind::EmptyMags
);
REGISTER_CHAOS_EFFECT_PARAM(
    SingleUseMags,
    ZMagazineUpdateModifierEffect,
    "SingleUseMags",
    "Single Shot",
    ZMagazineUpdateModifierEffect::EApplyTo::All,
    ZMagazineUpdateModifierEffect::EKind::SingleBulletMags
);
