#pragma once
#include "IChaosEffect.h"
#include "Helpers/CompanionMod.h"

class ZCompanionModDependentEffectBase : public virtual IChaosEffect
{
  public:
    ZCompanionModDependentEffectBase(const CompanionModUtil::SVersion p_MinimumVersion = CompanionModUtil::SVersion(0, 0, 0))
        : m_MinimumVersion(p_MinimumVersion)
    {
    }

    bool Available() const override;

  protected:
    /**
     * manually checks if a certain version of the companion mod is available.
     * this is for effects that generally work in older versions of the companion, but have additional features with newer versions.
     * this does NOT affect the availability status of the effect!
     * @param p_MinimumVersion The minimum version of the companion mod.
     * @return is at least that version installed?
     */
    bool HasCompanionMod(const CompanionModUtil::SVersion p_MinimumVersion) const;
    inline bool HasCompanionMod() const
    {
        return HasCompanionMod(m_MinimumVersion);
    }

  private:
    CompanionModUtil::SVersion m_MinimumVersion;
};
