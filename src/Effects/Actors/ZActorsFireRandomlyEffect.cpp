#include "ZActorsFireRandomlyEffect.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

void ZActorsFireRandomlyEffect::Start()
{
	SetAllActorsShootingContinuous(true);
}

void ZActorsFireRandomlyEffect::Stop()
{
	SetAllActorsShootingContinuous(false);
}

void ZActorsFireRandomlyEffect::SetAllActorsShootingContinuous(const bool p_bShooting)
{
	for (const auto& s_rActor : Utils::GetActors(false, false))
	{
		if (auto s_Helper = GetShootAtHelperFor(s_rActor))
		{
			if (p_bShooting)
			{
				// setting m_bUnnoticed to true makes actors not blame 47 for shooting.
				// imo it'd be unfair to disable this, but idk maybe someday.
				// same for death context, anything other than DC_NOT_HERO will cause guards to blame 47
				s_Helper.m_bUnnoticed = true;
				s_Helper.m_eDeathContext = EDeathContext::eDC_NOT_HERO;

				// enable friendly fire between actors
				s_Helper.m_bAllowActorHitActor = true;

				// set fire interval
				s_Helper.m_fMinTime = 0.5f;
				s_Helper.m_fMaxTime = 1.5f;

				// parent transform to actor, locate slightly in front
				if (auto s_rTargetOpt = s_Helper.m_rTarget; s_rTargetOpt.has_value())
				{
					auto s_rTarget = s_rTargetOpt.value();

					if (auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(s_rActor.m_entityRef))
					{
						Utils::SetProperty<TEntityRef<ZSpatialEntity>>(s_rTarget.m_entityRef, "m_eidParent", s_rActorSpatial);

						SMatrix43 s_mLocalTransform = SMatrix43();
						s_mLocalTransform.Trans.y = -5.f; // in front
						s_mLocalTransform.Trans.z = 1.2f;  // ~chest height
						Utils::SetProperty<SMatrix43>(s_rTarget.m_entityRef, "m_mTransform", s_mLocalTransform);
					}
				}

				// go!
				s_Helper.StartFireContinuous();
			}
			else
			{
				s_Helper.StopFireContinuous();
			}
		}
	}
}

REGISTER_CHAOS_EFFECT(ZActorsFireRandomlyEffect);
