#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZItem.h>
#include <Glacier/ZEntity.h>
#include <Glacier/ZMath.h>

#include "Helpers/HookingHelpers.h"

class ZMagazineUpdateModifierEffect : public IChaosEffect
{
  public:
    enum class EApplyTo
    {
        Player,
        Actor,
        All // Actors + Player
    };

    enum class EKind
    {
        UnlimitedMags,
        EmptyMags,
        SingleBulletMags
    };

    ZMagazineUpdateModifierEffect(
        const std::string p_sNameSuffix,
        const std::string s_sDisplayName,
        const EApplyTo p_eApplyTo,
        const EKind p_eKind
    )
        : m_sNameSuffix(p_sNameSuffix),
          m_sDisplayName(s_sDisplayName),
          m_eApplyTo(p_eApplyTo),
          m_eKind(p_eKind)
    {
        ADD_BASE_CLASS_DETOUR_INSTANCE();
    }

    ~ZMagazineUpdateModifierEffect()
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
    EApplyTo m_eApplyTo;
    EKind m_eKind;

  private:
    bool m_bActive = false;

    DECLARE_BASE_CLASS_INSTANCES_HELPER(ZMagazineUpdateModifierEffect);

    DECLARE_BASE_CLASS_DETOUR(
        ZMagazineUpdateModifierEffect,
        void,
        ZHM5ItemWeapon_SetBulletsInMagazine,
        IFirearm* th,
        int32_t p_nBullets
    );
};