#include "ZCompanionModDependentEffectBase.h"

bool ZCompanionModDependentEffectBase::Available() const
{
    return IChaosEffect::Available() && HasCompanionMod();
}

bool ZCompanionModDependentEffectBase::HasCompanionMod(const CompanionModUtil::SVersion p_MinimumVersion) const
{
    const auto s_Metadata = CompanionModUtil::LoadCompanionModInfo(false);

    return s_Metadata.m_bPresent
           && s_Metadata.m_Version >= p_MinimumVersion;
}
