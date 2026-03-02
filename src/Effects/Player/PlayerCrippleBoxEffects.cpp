#include "PlayerCrippleBoxEffects.h"

#include "EffectRegistry.h"
#include "Helpers/Utils.h"

void ZUnlimitedAmmoEffect::Start()
{
    auto s_rCripple = GetCrippleBox();
    s_rCripple.m_pInterfaceRef->m_bLimitedAmmo = false;

    ActivateCrippleBox();
}

REGISTER_CHAOS_EFFECT(ZUnlimitedAmmoEffect);

void ZPlayerPacifistEffect::Start()
{
    auto s_rCripple = GetCrippleBox();

    // no guns
    s_rCripple.m_pInterfaceRef->m_bAim = false;
    s_rCripple.m_pInterfaceRef->m_bFire = false;
    s_rCripple.m_pInterfaceRef->m_bBlindFire = false;

    // no close-combat
    s_rCripple.m_pInterfaceRef->m_bCloseCombat = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatStandard = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatChoke = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatFakeSwing = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatPistolFinish = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatProps = false;
    s_rCripple.m_pInterfaceRef->m_bCloseCombatSnapNeck = false;
    s_rCripple.m_pInterfaceRef->m_bFiberWireKill = false;
    s_rCripple.m_pInterfaceRef->m_bCoverTakedown = false;

    // no pushing people to their death
    s_rCripple.m_pInterfaceRef->m_bKickVictimOverLedge = false;
    s_rCripple.m_pInterfaceRef->m_bPushVictimThroughWindowAndRail = false;
    s_rCripple.m_pInterfaceRef->m_bPullVictimFromWindow = false;
    s_rCripple.m_pInterfaceRef->m_bGrabVictim = false;

    // no dumping helpless bodies
    s_rCripple.m_pInterfaceRef->m_bDumpBody = false;
    s_rCripple.m_pInterfaceRef->m_bThrowBodyOverRail = false;
    s_rCripple.m_pInterfaceRef->m_bDragBody = false;

    // no littering (or throwing stuff at people)
    s_rCripple.m_pInterfaceRef->m_bThrowItem = false;
    s_rCripple.m_pInterfaceRef->m_bVRThrow = false; // i assume this is ~= m_bThrowItem
    s_rCripple.m_pInterfaceRef->m_bDropItem = false;

    // no disguising (bc that's weird)
    s_rCripple.m_pInterfaceRef->m_bTakeClothes = false;

    // no interactions causing shenanigans
    s_rCripple.m_pInterfaceRef->m_bInteractions = false;

    ActivateCrippleBox();
}

REGISTER_CHAOS_EFFECT(ZPlayerPacifistEffect);

void ZPlayerForceCrouchEffect::Start()
{
    auto s_rCripple = GetCrippleBox();

    // disable standing up, forcing the player to crouch always
    s_rCripple.m_pInterfaceRef->m_bStandUp = false;

    // also disable fast movements for the fun of it
    s_rCripple.m_pInterfaceRef->m_bFastWalk = false;
    s_rCripple.m_pInterfaceRef->m_bRun = false;

    ActivateCrippleBox();
}

bool ZPlayerForceCrouchEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           && !Utils::IsInstanceOf<ZPlayerSlowMovementEffect>(p_pOther); // also uses m_bFastWalk and m_bRun
}

REGISTER_CHAOS_EFFECT(ZPlayerForceCrouchEffect);

void ZPlayerSlowMovementEffect::Start()
{
    auto s_rCripple = GetCrippleBox();

    s_rCripple.m_pInterfaceRef->m_bFastWalk = false;
    s_rCripple.m_pInterfaceRef->m_bRun = false;

    ActivateCrippleBox();
}

bool ZPlayerSlowMovementEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           && !Utils::IsInstanceOf<ZPlayerForceCrouchEffect>(p_pOther); // also uses m_bFastWalk and m_bRun
}

REGISTER_CHAOS_EFFECT(ZPlayerSlowMovementEffect);

void ZPlayerNoInventoryEffect::Start()
{
    auto s_rCripple = GetCrippleBox();

    s_rCripple.m_pInterfaceRef->m_bItems = false;
    s_rCripple.m_pInterfaceRef->m_bCanHolsterItems = false;

    // player could get around inventory block by picking up items. we can't have that :P
    s_rCripple.m_pInterfaceRef->m_bPickupItem = false;
    s_rCripple.m_pInterfaceRef->m_bDropItem = false;

    ActivateCrippleBox();
}

bool ZPlayerNoInventoryEffect::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther)
           && !Utils::IsInstanceOf<ZPlayerPacifistEffect>(p_pOther); // also uses m_bDropItem
}

REGISTER_CHAOS_EFFECT(ZPlayerNoInventoryEffect);

void ZPlayerNoTurnEffect::Start()
{
    auto s_rCripple = GetCrippleBox();

    // no turning, only move in a straight line.
    s_rCripple.m_pInterfaceRef->m_bTurn = false;
    s_rCripple.m_pInterfaceRef->m_bVRTurn = false; // i assume this is ~= m_bTurn

    ActivateCrippleBox();
}

REGISTER_CHAOS_EFFECT(ZPlayerNoTurnEffect);
