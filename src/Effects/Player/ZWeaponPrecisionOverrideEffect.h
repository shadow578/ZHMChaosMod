#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZItem.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

#include "Helpers/HookingHelpers.h"

class ZWeaponPrecisionOverrideEffect : public virtual IChaosEffect // virtual inheritance to allow for ZPlayerJuggernautEffect
{
  public:
    ZWeaponPrecisionOverrideEffect(
        const std::string p_sNameSuffix,
        const std::string s_sDisplayName,
        const float32 p_fPrecisionFactor
    )
        : m_sNameSuffix(p_sNameSuffix),
          m_sDisplayName(s_sDisplayName),
          m_fPrecisionFactor(p_fPrecisionFactor)
    {
        ADD_BASE_CLASS_DETOUR_INSTANCE();
    }

    ~ZWeaponPrecisionOverrideEffect()
    {
        REMOVE_BASE_CLASS_DETOUR_INSTANCE();
    }

    void OnModInitialized() override;
    void OnModUnload() override;
    bool Available() const override;
    bool IsCompatibleWith(const IChaosEffect* p_pOtherEffect) const override;

    void OnDrawDebugUI() override;

    void Start() override;
    void Stop() override;

    std::string GetName() const override
    {
        return IChaosEffect::GetName() + "_" + m_sNameSuffix;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_sDisplayName;
    }

  private:
    std::string m_sNameSuffix;
    std::string m_sDisplayName;
    float32 m_fPrecisionFactor;

  private:
    bool m_bActive = false;

    DECLARE_BASE_CLASS_INSTANCES_HELPER(ZWeaponPrecisionOverrideEffect);

    DECLARE_BASE_CLASS_DETOUR(
        ZWeaponPrecisionOverrideEffect,
        bool,
        ZHM5ItemWeapon_FireProjectiles,
        ZHM5ItemWeapon* th,
        bool bMayStartSound
    );
};