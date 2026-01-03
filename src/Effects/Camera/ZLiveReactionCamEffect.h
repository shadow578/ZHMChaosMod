#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZTemplateEntitySpawner.h"

class ZLiveReactionCamEffect : public IChaosEffect
{
public:
	ZLiveReactionCamEffect(const bool p_bTargetPlayer) :
		m_bTargetPlayer(p_bTargetPlayer)
	{
	}

	void LoadResources() override;
	void OnClearScene() override;
	void OnDrawDebugUI() override;

	bool Available() const override;

	void Start() override;
	void Stop() override;

	bool IsCompatibleWith(const IChaosEffect* p_pOther) const override;

	std::string GetName() const override
	{
		return IChaosEffect::GetName() + (m_bTargetPlayer ? "_player" : "_actor");
	}

	std::string GetDisplayName() const override
	{
		return m_bTargetPlayer ? "Live Agent 47 Reaction" : "Live Victim Reaction";
	}

	ELifecycleMethodFlag AlwaysActiveLifecycleMethods() const override
	{
		return IChaosEffect::AlwaysActiveLifecycleMethods() |
			ELifecycleMethodFlag::OnClearScene;
	}

private:
	const bool m_bTargetPlayer;

	std::unique_ptr<ZTemplateEntitySpawner> m_pPIPCameraSpawner;
	std::unique_ptr<ZResourceProvider> m_pHudMessageResource;

	ZEntityRef m_rPIPCameraEntity;

	TEntityRef<ZSpatialEntity> GetRandomActorHeadAttachEntity();
	TEntityRef<ZSpatialEntity> GetPlayerHeadAttachEntity();

	void SpawnLiveReactionCam(TEntityRef<ZSpatialEntity> p_rTargetHead, const ZRuntimeResourceID p_HudMessageId, const ZPIPMessageEntity_EIcon p_eIcon);
};