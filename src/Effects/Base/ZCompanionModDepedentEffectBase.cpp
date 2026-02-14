#include "ZCompanionModDepedentEffectBase.h"

bool ZCompanionModDepedentEffectBase::Available() const
{
	const auto s_Metadata = CompanionModUtil::LoadCompanionModInfo(false);

	return IChaosEffect::Available()
		&& s_Metadata.m_bPresent
		&& s_Metadata.m_Version >= m_MinimumVersion;
}
