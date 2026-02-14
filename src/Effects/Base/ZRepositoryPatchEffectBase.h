#pragma once
#include "IChaosEffect.h"

#include "Helpers/Repository/ZRepositoryEntryPatcher.h"

/**
 * Base class that allows patching the game repository.
 * Patches created by this effect will be automatically restored when the effect ends or the scene unloads.
 */
class ZRepositoryPatchEffectBase : public virtual IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;

	void Stop() override;

protected:
	std::shared_ptr<ZRepositoryEntryPatcher> Patch(const std::string& p_sId);
	std::shared_ptr<ZRepositoryEntryPatcher> Patch(const ZRepositoryID& p_RepoId);

private:
	std::vector<std::shared_ptr<ZRepositoryEntryPatcher>> m_aPatches;
};
