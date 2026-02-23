#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZActor.h>

#include <map>

/**
 * Reuseable base for monitoring of the wellbeing state of all active actors. 
 * Callback OnActorWellbeingStateChanged is called each time a actor is pacified or killed.
 */
class ZActorWellbeingChangeEffectBase : public virtual IChaosEffect
{
public:
    void Start() override;
    void Stop() override;
    void OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining) override;

protected:
    struct SActorState
    {
        /**
         * Is the Actor fully dead (not just pacified)? 
         * Note that this does NOT correspond to the IsDead() method of ZActor.
         */
        bool m_bDead;

        /**
         * Is the Actor pacified? 
         */
        bool m_bPacified;

        bool operator==(const SActorState& p_Other) const
        {
            return m_bDead == p_Other.m_bDead &&
                   m_bPacified == p_Other.m_bPacified;
        }
    };

    static SActorState GetActorState(ZActor* p_pActor);
    
    virtual void OnActorWellbeingChanged(ZActor* p_pActor, const SActorState& p_OldState, const SActorState& p_NewState) = 0;

private:
    bool m_bMonitorChanges = false;
    std::map<ZActor*, SActorState> m_mLastActorStates;
};
