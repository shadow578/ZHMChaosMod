#pragma once
#include "IChaosEffect.h"

#include "Helpers/ZTemplateEntitySpawner.h"

class ZFireworksEffect : public IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;
	bool Available() const override;
	void OnDrawDebugUI() override;

	void Start() override;
	void Stop() override;

private:
	std::unique_ptr<ZTemplateEntitySpawnerSession> m_pFireworksSpawner;
	ZEntityRef m_rFireworksEntity;
};
