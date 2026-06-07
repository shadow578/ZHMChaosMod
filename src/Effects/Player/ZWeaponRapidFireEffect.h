#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZItem.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

#include "Helpers/HookingHelpers.h"

class ZWeaponRapidFireEffect : public virtual IChaosEffect // virtual inheritance to allow for ZPlayerJuggernautEffect
{
  public:
    ZWeaponRapidFireEffect()
    {
        ADD_BASE_CLASS_DETOUR_INSTANCE();
    }

    ~ZWeaponRapidFireEffect()
    {
        REMOVE_BASE_CLASS_DETOUR_INSTANCE();
    }

    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Rapid Fire";
    }

  private:
    bool m_bActive = false;

    DECLARE_BASE_CLASS_INSTANCES_HELPER(ZWeaponRapidFireEffect);

    DECLARE_BASE_CLASS_DETOUR(
        ZWeaponRapidFireEffect,
        bool,
        ZHM5ItemWeapon_IsFiring,
        IFirearm* th
    );
};