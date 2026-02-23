#pragma once
#include "Effects/Base/ZCameraEffectBase.h"
#include "Effects/Base/Companion/ZActorFollowPlayerHelperEffectBase.h"

class ZFollowingActorCameraEffect :
	public ZCameraEffectBase,
	public ZActorFollowPlayerHelperEffectBase
{
public:
	ZFollowingActorCameraEffect():
		ZCameraEffectBase(),
		ZActorFollowPlayerHelperEffectBase()
	{
	}

	bool Available() const override;
	void OnDrawDebugUI() override;

	void Start() override;
	void Stop() override;
	void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining) override;

	std::string GetDisplayName(const bool p_bVoting) const override
	{
		return "NPC POV";
	}

	EDuration GetDuration() const override
	{
		return EDuration::Short;
	}

private:
	TEntityRef<ZActor> m_rFollowingActor;
	TEntityRef<ZSpatialEntity> m_rFollowingActorHeadAttach;

	void MakeActorFollowingCameraPerson(const TEntityRef<ZActor> p_rActor);
};
