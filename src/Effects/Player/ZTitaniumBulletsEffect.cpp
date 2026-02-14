#include "ZTitaniumBulletsEffect.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZTitaniumBulletsEffect] "

void ZTitaniumBulletsEffect::Start()
{
	for (const ZRepositoryID& s_RepoId : ZHMRepositoryHelper::GetInstance().GetEntryIdsByType(ZHMRepositoryHelper::EEntryType::MagazineConfig))
	{
		auto s_pPatcher = Patch(s_RepoId);
		Logger::Debug(TAG "Patching MagazineConfig entry {}",s_RepoId.ToString().c_str());

		const auto s_sOriginalAmmoConfigOpt = s_pPatcher->Get<ZString>("AmmoConfig");
		Logger::Debug(TAG " - Original AmmoConfig: {}", s_sOriginalAmmoConfigOpt.has_value() ? s_sOriginalAmmoConfigOpt.value()->c_str() : "<null>");

		if (!s_pPatcher->Set<ZString>("AmmoConfig", "87ae0524-2f22-4fe0-82e1-84a050b43cf0"))
		{
			Logger::Debug(TAG "failed to set AmmoConfig!");
			continue;
		}

		const auto s_sNewAmmoConfigOpt = s_pPatcher->Get<ZString>("AmmoConfig");
		Logger::Debug(TAG " - New AmmoConfig: {}", s_sNewAmmoConfigOpt.has_value() ? s_sNewAmmoConfigOpt.value()->c_str() : "<null>");
	}
}

REGISTER_CHAOS_EFFECT(ZTitaniumBulletsEffect);
