#include "ZActorsFollowPlayerEffect.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"

void ZActorsFollowPlayerEffect::Start()
{
	SignalAllActors("CM_StartFollow");
}

void ZActorsFollowPlayerEffect::Stop()
{
	SignalAllActors("CM_StopFollow");
}

void ZActorsFollowPlayerEffect::SignalAllActors(const std::string& p_sSignalName)
{
	for (const auto s_pActor : Utils::GetActors(false, false))
	{
		if (!s_pActor) continue;

		ZEntityRef s_rActor;
		s_pActor->GetID(s_rActor);

		if (!s_rActor) continue;

		s_rActor.SignalInputPin(p_sSignalName);
	}
}

REGISTER_CHAOS_EFFECT(ZActorsFollowPlayerEffect);
