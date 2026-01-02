#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>

#include <Helpers/ZTemplateEntitySpawner.h>

class ZRenderPostfilerEffect : public IChaosEffect
{
public:
	void LoadResources() override;

	void OnActivatingScene() override;
	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start() override;
	void Stop() override;

private:
	ZEntityRef m_rPostfilterGraphEntity;
	bool m_bPostfilterGraphEntityLoaded = false;

	std::unique_ptr<ZTemplateEntitySpawnerSession> m_pCustomLayer;
};
