#include "ZActorWellbeingChangeEffectBase.h"

#include "Helpers/ActorUtils.h"

void ZActorWellbeingChangeEffectBase::Start()
{
    // note: next frame update will populate the initial states
    m_mLastActorStates.clear();
    m_bMonitorChanges = true;
}

void ZActorWellbeingChangeEffectBase::Stop()
{
    m_mLastActorStates.clear();
    m_bMonitorChanges = false;
}

void ZActorWellbeingChangeEffectBase::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (!m_bMonitorChanges)
    {
        return;
    }

    for (auto& s_rActor : Utils::GetActors(true, true))
    {
		const auto s_CurrentState = GetActorState(s_rActor.m_pInterfaceRef);

        auto s_it = m_mLastActorStates.find(s_rActor.m_pInterfaceRef);
        if (s_it == m_mLastActorStates.end())
        {
            m_mLastActorStates[s_rActor.m_pInterfaceRef] = s_CurrentState;
            continue;
        }

        const auto s_LastState = s_it->second;
        if (s_CurrentState == s_LastState)
        {
            continue;
        }

        m_mLastActorStates[s_rActor.m_pInterfaceRef] = s_CurrentState;

        OnActorWellbeingChanged(s_rActor.m_pInterfaceRef, s_LastState, s_CurrentState);
    }
}

ZActorWellbeingChangeEffectBase::SActorState ZActorWellbeingChangeEffectBase::GetActorState(ZActor* p_pActor)
{
    // ZActor's methods for detecting the state of actors are quite unintuitive:
    // - when Pacified: IsDead() = true, IsAlive = false, IsPacified() = true, m_bAlive = false
    // - when Killed:   IsDead() = true, IsAlive = false, IsPacified() = false, m_bAlive = false
    //
    // i gues for IOI, a pacified actor is considered "dead" in the sense that they cannot do anything...
    // for our purposes, we'd want a clear distinction between dead and pacified actors.
	// thus, this method does some additional checks to determine the actual state of the actor, which is then used for comparison in the effect logic.

	const auto s_bIsPacified = p_pActor->IsPacified();
    const auto s_bIsDead = p_pActor->IsDead();

    return {
        .m_bDead = s_bIsDead && !s_bIsPacified, // only consider dead when not pacified
        .m_bPacified = s_bIsPacified
    };
}
