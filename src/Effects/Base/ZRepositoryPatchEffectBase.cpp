#include "ZRepositoryPatchEffectBase.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZRepositoryPatchEffectBase] "

void ZRepositoryPatchEffectBase::LoadResources()
{
	ZHMRepositoryHelper::GetInstance().Initialize();
}

void ZRepositoryPatchEffectBase::OnClearScene()
{
	ZHMRepositoryHelper::GetInstance().Reset();
}

void ZRepositoryPatchEffectBase::Start()
{
	auto& s_Repo = ZHMRepositoryHelper::GetInstance();

	for (const ZRepositoryID& s_RepoId : s_Repo.GetEntryIdsByType(ZHMRepositoryHelper::EEntryType::MagazineConfig))
	{
		auto s_pEntry = s_Repo.Get(s_RepoId);

		const auto s_sIDOpt = s_pEntry->Get<ZString>("ID_");
		const auto s_sID = s_sIDOpt.has_value() ? s_sIDOpt.value()->c_str() : "<null>";

		Logger::Debug(TAG "found MagazineConfig entry: {} (ID_ = {}", 
			s_RepoId.ToString().c_str(),
			s_sID
		);


		const auto s_sOriginalAmmoConfigOpt = s_pEntry->Get<ZString>("AmmoConfig");
		Logger::Debug(TAG " - Original AmmoConfig: {}", s_sOriginalAmmoConfigOpt.has_value() ? s_sOriginalAmmoConfigOpt.value()->c_str() : "<null>");

		if (!s_pEntry->Set<ZString>("AmmoConfig", "87ae0524-2f22-4fe0-82e1-84a050b43cf0"))
		{
			Logger::Debug(TAG "Set AmmoConfig failed!");
		}
	}

}

void ZRepositoryPatchEffectBase::Stop()
{
}

REGISTER_CHAOS_EFFECT(ZRepositoryPatchEffectBase);
