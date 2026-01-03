#include "ZInvisibleActorsEffect.h"

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

void ZInvisibleActorsEffect::Start()
{
    ZMinimapNoActorsEffect::Start();
    SetAllActorsVisibility(false);
}

void ZInvisibleActorsEffect::Stop()
{
    ZMinimapNoActorsEffect::Stop();
    SetAllActorsVisibility(true);
}

void ZInvisibleActorsEffect::SetAllActorsVisibility(const bool p_bVisible)
{
    for (auto* s_pActor : Utils::GetActors(true, true))
    {
        if (!s_pActor)
        {
            continue;
        }

        ZEntityRef s_ActorRef;
        s_pActor->GetID(s_ActorRef);

        Utils::SetProperty<bool>(s_ActorRef, "m_bVisible", p_bVisible);
    }
}

REGISTER_CHAOS_EFFECT(ZInvisibleActorsEffect)
