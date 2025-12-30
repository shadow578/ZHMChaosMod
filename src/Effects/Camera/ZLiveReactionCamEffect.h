#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZTemplateEntitySpawner.h"

class ZLiveReactionCamEffect : public IChaosEffect
{
public:
	void LoadResources() override;
	void OnClearScene() override;
	void OnDrawDebugUI() override;

	bool Available() const override;

	void Start() override;
	void Stop() override;

private:
	std::unique_ptr<ZTemplateEntitySpawnerSession> m_pPIPCameraSpawner;
	ZEntityRef m_rPIPCameraEntity;

	TEntityRef<ZSpatialEntity> GetRandomActorHeadAttachEntity();
	TEntityRef<ZSpatialEntity> GetPlayerHeadAttachEntity();

	void SpawnLiveReactionCam(TEntityRef<ZSpatialEntity> p_rTargetHead, const ZRuntimeResourceID p_HudMessageId);
};