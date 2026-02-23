#include "ZActorsFollowPlayerEffect.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"

void ZActorsFollowPlayerEffect::Start()
{
	SetActorsFollowPlayer(true);
}

void ZActorsFollowPlayerEffect::Stop()
{
	SetActorsFollowPlayer(false);
}

void ZActorsFollowPlayerEffect::SetActorsFollowPlayer(const bool p_bFollow)
{
	for (const auto &s_rActor : Utils::GetActors(false, false))
	{
		if (!s_rActor) continue;

		if (auto s_FollowHelper = GetFollowHelperFor(s_rActor); s_FollowHelper)
		{
			if (p_bFollow)
			{
				// make following actors ignore sillyness
				s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreAnnoyingHitman = true;
				s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreSillyHitman = true;

				s_FollowHelper.m_fMinTetherRange = 2.f;
				s_FollowHelper.m_fMaxTetherRange = 5.f;

				// go!
				s_FollowHelper.StartFollowHitman();
			}
			else
			{
				s_FollowHelper.StopFollowHitman();
			}
		}
	}
}

REGISTER_CHAOS_EFFECT(ZActorsFollowPlayerEffect);
