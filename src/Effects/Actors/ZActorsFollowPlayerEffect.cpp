#include "ZActorsFollowPlayerEffect.h"

#include "Registry.h"
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
    for (const auto& s_rActor : Utils::GetActors(false, false))
    {
        if (!s_rActor)
            continue;

        if (auto s_FollowHelper = GetFollowHelperFor(s_rActor); s_FollowHelper)
        {
            if (p_bFollow)
            {
                // make following actors ignore sillyness
                s_FollowHelper.m_AIModifierBinding.m_bIgnoreAnnoyingHitman = true;
                s_FollowHelper.m_AIModifierBinding.m_bIgnoreSillyHitman = true;

                s_FollowHelper.m_fMinTetherRange = 2.f;
                s_FollowHelper.m_fMaxTetherRange = 5.f;

                // go extra fast to be a lot more unsetteling
                s_FollowHelper.m_eMaxMoveSpeed = EMoveSpeed::MS_Flash;

                // go!
                if (m_bActorsIgnoreAllElse)
                {
                    s_FollowHelper.StartFollowHitmanIgnoreEverything();
                }
                else
                {
                    s_FollowHelper.StartFollowHitman();
                }
            }
            else
            {
                if (m_bActorsIgnoreAllElse)
                {
                    s_FollowHelper.StopFollowHitmanIgnoreEverything();
                }
                else
                {
                    s_FollowHelper.StopFollowHitman();
                }
            }
        }
    }
}

REGISTER_CHAOS_EFFECT_PARAM(normal, ZActorsFollowPlayerEffect, /* Ignore All Else? */ false);
REGISTER_CHAOS_EFFECT_PARAM(nobrain, ZActorsFollowPlayerEffect, /* Ignore All Else? */ true);
