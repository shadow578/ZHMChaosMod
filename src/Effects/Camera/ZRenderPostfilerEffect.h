#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>

class ZRenderPostfilerEffect : public IChaosEffect
{
public:
	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void Start() override;
	void Stop() override;

private:
	ZEntityRef m_rPostfilterLayerEntity;
	ZEntityRef m_rPostfilterParametersEntity;
	bool m_bPostfilterLoaded = false;
};
