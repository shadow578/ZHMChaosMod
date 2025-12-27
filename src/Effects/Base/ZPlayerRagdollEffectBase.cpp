#include "ZPlayerRagdollEffectBase.h"

#include "IPluginInterface.h"
#include "Functions.h"

#include <Glacier/ZHitman5.h>

#include "Helpers/Utils.h"

void ZPlayerRagdollEffectBase::Start()
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player.m_pInterfaceRef)
    {
        return;
    }

    if (m_bPowered)
    {
        Functions::ZHM5BaseCharacter_ActivatePoweredRagdoll->Call(
            s_Player.m_pInterfaceRef,
            0,
            false,
            true,
            0,
            true
        );
    }
    else
    {
        Functions::ZHM5BaseCharacter_ActivateRagdoll->Call(s_Player.m_pInterfaceRef, false);
    }
}

void ZPlayerRagdollEffectBase::Stop()
{
    auto s_Player = SDK()->GetLocalPlayer();
    if (!s_Player.m_pInterfaceRef)
    {
        return;
    }

    Functions::ZHM5BaseCharacter_DeactivateRagdoll->Call(s_Player.m_pInterfaceRef);

    // play animation to get up
    auto s_pAnimator = s_Player.m_pInterfaceRef->m_Animator.QueryInterface<ZHM5Animator>();
    auto s_fTime = 1.0f;

    if (s_pAnimator && Functions::ZHM5Animator_ActivateRagdollToAnimationBlend->Exists())
    {
        Functions::ZHM5Animator_ActivateRagdollToAnimationBlend->Call(s_pAnimator, &s_fTime);
    }
}

bool ZPlayerRagdollEffectBase::Available()
{

    return (m_bPowered ?
        Functions::ZHM5BaseCharacter_ActivatePoweredRagdoll->Exists() :
        Functions::ZHM5BaseCharacter_ActivateRagdoll->Exists()
        ) &&
        Functions::ZHM5BaseCharacter_DeactivateRagdoll->Exists();
}

bool ZPlayerRagdollEffectBase::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
        // all ragdoll effects are incompatible with each other
        && !Utils::IsInstanceOf<ZPlayerRagdollEffectBase>(p_pOther);
}
