#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

#include "Helpers/HookingHelpers.h"

class ZRecoilModifierEffect : public virtual IChaosEffect // virtual inheritance to allow for ZPlayerJuggernautEffect
{
  public:
    ZRecoilModifierEffect(
        const std::string p_sNameSuffix,
        const std::string s_sDisplayName,
        const SVector2 p_vRecoilMultiplier,
        const EDuration p_eDuration = EDuration::Full
    )
        : m_sNameSuffix(p_sNameSuffix),
          m_sDisplayName(s_sDisplayName),
          m_vRecoilMultiplier(p_vRecoilMultiplier),
          m_eDuration(p_eDuration)
    {
        ADD_BASE_CLASS_DETOUR_INSTANCE();
    }

    ~ZRecoilModifierEffect()
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

    EDuration GetDuration() const override
    {
        return m_eDuration;
    }

  private:
    std::string m_sNameSuffix;
    std::string m_sDisplayName;
    EDuration m_eDuration;
    SVector2 m_vRecoilMultiplier;

  private:
    bool m_bActive = false;

    DECLARE_BASE_CLASS_INSTANCES_HELPER(ZRecoilModifierEffect);

    DECLARE_BASE_CLASS_DETOUR(
        ZRecoilModifierEffect,
        void,
        ZHM5WeaponRecoilController_RecoilWeapon,
        ZHM5WeaponRecoilController* th,
        const TEntityRef<ZHM5ItemWeapon>& p_rWeapon
    );
};
