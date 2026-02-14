#pragma once
#include "IChaosEffect.h"

class ZRepositoryPatchEffectBase : public virtual IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;

	void Start() override;
	void Stop() override;
};
