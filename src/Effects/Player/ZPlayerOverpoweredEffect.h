#pragma once
#include "Effects/Player/ZRecoilModifierEffect.h"
#include "Effects/Player/ZMagazineUpdateModifierEffect.h"
#include "Effects/Player/ZWeaponPrecisionOverrideEffect.h"
#include "Effects/Player/ZWeaponRapidFireEffect.h"

// this is such a stupid effect lol
// this effect combines four other effects to create a "juggernaut" mode for the player.
// you can probably see how stupidly overpowered this is :P
// Note: some of the sub-effects define name & display name via constructor parameters (for use with REGISTER_CHAOS_EFFECT_PARAM).
// here, we simply pass dummy values, as we override the methods anyway.
class ZPlayerOverpoweredEffect : public ZRecoilModifierEffect, public ZMagazineUpdateModifierEffect, public ZWeaponPrecisionOverrideEffect, public ZWeaponRapidFireEffect
{
  public:
    ZPlayerOverpoweredEffect()
        : ZRecoilModifierEffect("op", "", SVector2(0.f, 0.f)),
          ZMagazineUpdateModifierEffect("op", "", EApplyTo::Player, EKind::UnlimitedMags),
          ZWeaponPrecisionOverrideEffect("op", "", 1.f),
          ZWeaponRapidFireEffect()
    {
    }

    void OnModInitialized() override
    {
        ZRecoilModifierEffect::OnModInitialized();
        ZMagazineUpdateModifierEffect::OnModInitialized();
        ZWeaponPrecisionOverrideEffect::OnModInitialized();
        ZWeaponRapidFireEffect::OnModInitialized();
    }

    void OnModUnload() override
    {
        ZRecoilModifierEffect::OnModUnload();
        ZMagazineUpdateModifierEffect::OnModUnload();
        ZWeaponPrecisionOverrideEffect::OnModUnload();
        ZWeaponRapidFireEffect::OnModUnload();
    }

    bool Available() const override
    {
        return ZRecoilModifierEffect::Available()
               && ZMagazineUpdateModifierEffect::Available()
               && ZWeaponPrecisionOverrideEffect::Available()
               && ZWeaponRapidFireEffect::Available();
    }

    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override
    {
        return ZRecoilModifierEffect::IsCompatibleWith(p_pOtherEffect)
               && ZMagazineUpdateModifierEffect::IsCompatibleWith(p_pOtherEffect)
               && ZWeaponPrecisionOverrideEffect::IsCompatibleWith(p_pOtherEffect)
               && ZWeaponRapidFireEffect::IsCompatibleWith(p_pOtherEffect);
    }

    void OnDrawDebugUI() override
    {
        ZRecoilModifierEffect::OnDrawDebugUI();
        ZMagazineUpdateModifierEffect::OnDrawDebugUI();
        ZWeaponPrecisionOverrideEffect::OnDrawDebugUI();
        ZWeaponRapidFireEffect::OnDrawDebugUI();
    }

    void Start() override
    {
        ZRecoilModifierEffect::Start();
        ZMagazineUpdateModifierEffect::Start();
        ZWeaponPrecisionOverrideEffect::Start();
        ZWeaponRapidFireEffect::Start();
    }

    void Stop() override
    {
        ZRecoilModifierEffect::Stop();
        ZMagazineUpdateModifierEffect::Stop();
        ZWeaponPrecisionOverrideEffect::Stop();
        ZWeaponRapidFireEffect::Stop();
    }

    std::string GetName() const override
    {
        return IChaosEffect::GetName();
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Unfair Advantage"; // TODO name kinda sucks thb
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }
};
