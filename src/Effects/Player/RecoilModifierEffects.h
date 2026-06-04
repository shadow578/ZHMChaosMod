#pragma once
#include "IChaosEffect.h"

#include <IPluginInterface.h> // for DECLARE_PLUGIN_DETOUR

#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

// NOTE: because this uses detours, each variant must define its own class implementing and handling the detour.
// this is because detours are registered via the method pointer, and must be unique.

class ZRecoilModifierEffect : public IChaosEffect
{
  public:
    ZRecoilModifierEffect(const SVector2 p_vRecoilMultiplier) : m_vRecoilMultiplier(p_vRecoilMultiplier)
    {
    }

    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

  protected:
    SVector2 m_vRecoilMultiplier;
    bool m_bActive = false;
};

class ZNoRecoilEffect : public ZRecoilModifierEffect
{
  public:
    ZNoRecoilEffect() : ZRecoilModifierEffect(SVector2(0.f, 0.f))
    {
    }

    void OnModInitialized() override;
    void OnModUnload() override;

  private:
    DECLARE_PLUGIN_DETOUR(
        ZNoRecoilEffect,
        void,
        ZHM5WeaponRecoilController_RecoilWeapon,
        ZHM5WeaponRecoilController* th,
        const TEntityRef<ZHM5ItemWeapon>& p_rWeapon
    );
};

class ZExtremeRecoilEffect : public ZRecoilModifierEffect
{
  public:
    ZExtremeRecoilEffect() : ZRecoilModifierEffect(SVector2(5.f, 5.f))
    {
    }

    void OnModInitialized() override;
    void OnModUnload() override;

  private:
    DECLARE_PLUGIN_DETOUR(
        ZExtremeRecoilEffect,
        void,
        ZHM5WeaponRecoilController_RecoilWeapon,
        ZHM5WeaponRecoilController* th,
        const TEntityRef<ZHM5ItemWeapon>& p_rWeapon
    );
};
