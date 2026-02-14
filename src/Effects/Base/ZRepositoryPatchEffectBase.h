#pragma once
#include "IChaosEffect.h"

#include "Helpers/Repository/ZRepositoryEntryPatcher.h"

class ZRepositoryPatchEffectBase : public virtual IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;

	void Start() override;
	void Stop() override;

protected:
	std::shared_ptr<ZRepositoryEntryPatcher> Patch(const std::string& p_sId);
	std::shared_ptr<ZRepositoryEntryPatcher> Patch(const ZRepositoryID& p_RepoId);

private:
	std::vector<std::shared_ptr<ZRepositoryEntryPatcher>> m_aPatches;
};
