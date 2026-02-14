#include "ZRepositoryPatchEffectBase.h"

#include <Logging.h>

#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZRepositoryPatchEffectBase] "

void ZRepositoryPatchEffectBase::LoadResources()
{
	ZHMRepositoryHelper::GetInstance().Initialize();
}

void ZRepositoryPatchEffectBase::OnClearScene()
{
	m_aPatches.clear();

	ZHMRepositoryHelper::GetInstance().Reset();
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
