#include "ZRepositoryPatchEffectBase.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#include "Helpers/Repository/ZRepositoryEntryPatcher.h"

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
		auto s_pPatcher = Patch(s_RepoId);

		const auto s_sIDOpt = s_pPatcher->Get<ZString>("ID_");
		const auto s_sID = s_sIDOpt.has_value() ? s_sIDOpt.value()->c_str() : "<null>";

		Logger::Debug(TAG "found MagazineConfig entry: {} (ID_ = {}",
			s_RepoId.ToString().c_str(),
			s_sID
		);


		const auto s_sOriginalAmmoConfigOpt = s_pPatcher->Get<ZString>("AmmoConfig");
		Logger::Debug(TAG " - Original AmmoConfig: {}", s_sOriginalAmmoConfigOpt.has_value() ? s_sOriginalAmmoConfigOpt.value()->c_str() : "<null>");

		if (!s_pPatcher->Set<ZString>("AmmoConfig", "87ae0524-2f22-4fe0-82e1-84a050b43cf0"))
		{
			Logger::Debug(TAG "Set AmmoConfig failed!");
		}

		const auto s_sNewAmmoConfigOpt = s_pPatcher->Get<ZString>("AmmoConfig");
		Logger::Debug(TAG " - New AmmoConfig: {}", s_sNewAmmoConfigOpt.has_value() ? s_sNewAmmoConfigOpt.value()->c_str() : "<null>");


		/*
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
		*/
	}

}

void ZRepositoryPatchEffectBase::Stop()
{
	// this will release all patchers, which will in turn restore all patched entries 
	// to their original values via their destructors
	m_aPatches.clear();
}


std::shared_ptr<ZRepositoryEntryPatcher> ZRepositoryPatchEffectBase::Patch(const std::string& p_sId)
{
	auto s_pPatcher = ZHMRepositoryHelper::GetInstance().Patch(p_sId);
	if (!s_pPatcher)
	{
		Logger::Debug(TAG "Failed to create patcher for ID_ = {}", p_sId.c_str());
		return nullptr;
	}

	std::shared_ptr<ZRepositoryEntryPatcher> s_pSharedPatcher = std::move(s_pPatcher);

	m_aPatches.push_back(s_pSharedPatcher);
	return s_pSharedPatcher;
}

std::shared_ptr<ZRepositoryEntryPatcher> ZRepositoryPatchEffectBase::Patch(const ZRepositoryID& p_RepoId)
{
	auto s_pPatcher = ZHMRepositoryHelper::GetInstance().Patch(p_RepoId);
	if (!s_pPatcher)
	{
		Logger::Debug(TAG "Failed to create patcher for RepoId = {}", p_RepoId.ToString().c_str());
		return nullptr;
	}

	std::shared_ptr<ZRepositoryEntryPatcher> s_pSharedPatcher = std::move(s_pPatcher);

	m_aPatches.push_back(s_pSharedPatcher);
	return s_pSharedPatcher;
}

REGISTER_CHAOS_EFFECT(ZRepositoryPatchEffectBase);
