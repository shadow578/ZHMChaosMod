#include "ZPlayerCrippleBoxEffectBase.h"

#include <Glacier/ZHM5CrippleBox.h>

void ZPlayerCrippleBoxEffectBase::LoadResources()
{
    m_pCrippleBoxSpawner = ZTemplateEntitySpawner::Create<"[modules:/zhm5cripplebox.class].pc_entitytype">();
}

void ZPlayerCrippleBoxEffectBase::OnEnterScene()
{
    if (m_pCrippleBoxSpawner && m_pCrippleBoxSpawner->IsAvailable())
    {
        m_rCrippleBox = m_pCrippleBoxSpawner->SpawnAs<ZHM5CrippleBox>();
    }
}

void ZPlayerCrippleBoxEffectBase::OnClearScene()
{
    m_rCrippleBox = {};
    m_pCrippleBoxSpawner = nullptr;
}

bool ZPlayerCrippleBoxEffectBase::Available() const
{
    return IChaosEffect::Available()
           && m_pCrippleBoxSpawner && m_pCrippleBoxSpawner->IsAvailable()
           && m_rCrippleBox;
}

void ZPlayerCrippleBoxEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pCrippleBoxSpawner->ToString()).c_str());
    ImGui::TextUnformatted(fmt::format("CrippleBox Entity: {}", m_rCrippleBox ? "Spawned" : "Not Spawned").c_str());

    ImGui::SeparatorText("Configuration");

    if (!m_rCrippleBox)
    {
        ImGui::TextUnformatted("CrippleBox not spawned, cannot display configuration!");
        return;
    }

#pragma region Config Flags
    ImGui::Checkbox("m_bInSequence", &m_rCrippleBox.m_pInterfaceRef->m_bInSequence);
    ImGui::Checkbox("m_bAllowBaseMovements", &m_rCrippleBox.m_pInterfaceRef->m_bAllowBaseMovements);
    ImGui::Checkbox("m_bSequenceAllowCamera", &m_rCrippleBox.m_pInterfaceRef->m_bSequenceAllowCamera);
    ImGui::Checkbox("m_bActivateOnStart", &m_rCrippleBox.m_pInterfaceRef->m_bActivateOnStart);
    ImGui::Checkbox("m_bLedges", &m_rCrippleBox.m_pInterfaceRef->m_bLedges);
    ImGui::Checkbox("m_bCover", &m_rCrippleBox.m_pInterfaceRef->m_bCover);
    ImGui::Checkbox("m_bTakeClothes", &m_rCrippleBox.m_pInterfaceRef->m_bTakeClothes);
    ImGui::Checkbox("m_bLadder", &m_rCrippleBox.m_pInterfaceRef->m_bLadder);
    ImGui::Checkbox("m_bPullVictimFromWindow", &m_rCrippleBox.m_pInterfaceRef->m_bPullVictimFromWindow);
    ImGui::Checkbox("m_bFiberWireKill", &m_rCrippleBox.m_pInterfaceRef->m_bFiberWireKill);
    ImGui::Checkbox("m_bClimbWindow", &m_rCrippleBox.m_pInterfaceRef->m_bClimbWindow);
    ImGui::Checkbox("m_bThrowBodyOverRail", &m_rCrippleBox.m_pInterfaceRef->m_bThrowBodyOverRail);
    ImGui::Checkbox("m_bDumpBody", &m_rCrippleBox.m_pInterfaceRef->m_bDumpBody);
    ImGui::Checkbox("m_bOperateCPDoor", &m_rCrippleBox.m_pInterfaceRef->m_bOperateCPDoor);
    ImGui::Checkbox("m_bHideInCloset", &m_rCrippleBox.m_pInterfaceRef->m_bHideInCloset);
    ImGui::Checkbox("m_bCloseCombat", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombat);
    ImGui::Checkbox("m_bGrabVictim", &m_rCrippleBox.m_pInterfaceRef->m_bGrabVictim);
    ImGui::Checkbox("m_bPushVictimThroughWindowAndRail", &m_rCrippleBox.m_pInterfaceRef->m_bPushVictimThroughWindowAndRail);
    ImGui::Checkbox("m_bKickVictimOverLedge", &m_rCrippleBox.m_pInterfaceRef->m_bKickVictimOverLedge);
    ImGui::Checkbox("m_bPickupItem", &m_rCrippleBox.m_pInterfaceRef->m_bPickupItem);
    ImGui::Checkbox("m_bDropItem", &m_rCrippleBox.m_pInterfaceRef->m_bDropItem);
    ImGui::Checkbox("m_bDragBody", &m_rCrippleBox.m_pInterfaceRef->m_bDragBody);
    ImGui::Checkbox("m_bThrowItem", &m_rCrippleBox.m_pInterfaceRef->m_bThrowItem);
    ImGui::Checkbox("m_bPlaceItem", &m_rCrippleBox.m_pInterfaceRef->m_bPlaceItem);
    ImGui::Checkbox("m_bInteractions", &m_rCrippleBox.m_pInterfaceRef->m_bInteractions);
    ImGui::Checkbox("m_bUseDeathAnimation", &m_rCrippleBox.m_pInterfaceRef->m_bUseDeathAnimation);
    ImGui::Checkbox("m_bVRTurn", &m_rCrippleBox.m_pInterfaceRef->m_bVRTurn);
    ImGui::Checkbox("m_bVRThrow", &m_rCrippleBox.m_pInterfaceRef->m_bVRThrow);
    ImGui::Checkbox("m_bRun", &m_rCrippleBox.m_pInterfaceRef->m_bRun);
    ImGui::Checkbox("m_bTurn", &m_rCrippleBox.m_pInterfaceRef->m_bTurn);
    ImGui::Checkbox("m_bSneak", &m_rCrippleBox.m_pInterfaceRef->m_bSneak);
    ImGui::Checkbox("m_bNoSnapSneak", &m_rCrippleBox.m_pInterfaceRef->m_bNoSnapSneak);
    ImGui::Checkbox("m_bStandUp", &m_rCrippleBox.m_pInterfaceRef->m_bStandUp);
    ImGui::Checkbox("m_bFastWalk", &m_rCrippleBox.m_pInterfaceRef->m_bFastWalk);
    ImGui::Checkbox("m_bMovementAllowed", &m_rCrippleBox.m_pInterfaceRef->m_bMovementAllowed);
    ImGui::Checkbox("m_bIdleAnimationsAllowed", &m_rCrippleBox.m_pInterfaceRef->m_bIdleAnimationsAllowed);
    ImGui::Checkbox("m_bItems", &m_rCrippleBox.m_pInterfaceRef->m_bItems);
    ImGui::Checkbox("m_bCanHolsterItems", &m_rCrippleBox.m_pInterfaceRef->m_bCanHolsterItems);
    ImGui::Checkbox("m_bCoverTakedown", &m_rCrippleBox.m_pInterfaceRef->m_bCoverTakedown);
    ImGui::Checkbox("m_bCoverScale", &m_rCrippleBox.m_pInterfaceRef->m_bCoverScale);
    ImGui::Checkbox("m_bCoverToCover", &m_rCrippleBox.m_pInterfaceRef->m_bCoverToCover);
    ImGui::Checkbox("m_bCloseCombatSnapNeck", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatSnapNeck);
    ImGui::Checkbox("m_bCloseCombatChoke", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatChoke);
    ImGui::Checkbox("m_bCloseCombatPistolFinish", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatPistolFinish);
    ImGui::Checkbox("m_bCloseCombatProps", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatProps);
    ImGui::Checkbox("m_bCloseCombatStandard", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatStandard);
    ImGui::Checkbox("m_bCloseCombatFakeSwing", &m_rCrippleBox.m_pInterfaceRef->m_bCloseCombatFakeSwing);
    ImGui::Checkbox("m_bGameCameraAutoAlign", &m_rCrippleBox.m_pInterfaceRef->m_bGameCameraAutoAlign);
    ImGui::Checkbox("m_bCameraSide", &m_rCrippleBox.m_pInterfaceRef->m_bCameraSide);
    ImGui::Checkbox("m_bNotSelfieMode", &m_rCrippleBox.m_pInterfaceRef->m_bNotSelfieMode);
    ImGui::Checkbox("m_bInstinct", &m_rCrippleBox.m_pInterfaceRef->m_bInstinct);
    ImGui::Checkbox("m_bBlindFire", &m_rCrippleBox.m_pInterfaceRef->m_bBlindFire);
    ImGui::Checkbox("m_bAim", &m_rCrippleBox.m_pInterfaceRef->m_bAim);
    ImGui::Checkbox("m_bHairTrigger", &m_rCrippleBox.m_pInterfaceRef->m_bHairTrigger);
    ImGui::Checkbox("m_bFire", &m_rCrippleBox.m_pInterfaceRef->m_bFire);
    ImGui::Checkbox("m_bLimitedAmmo", &m_rCrippleBox.m_pInterfaceRef->m_bLimitedAmmo);
    ImGui::Checkbox("m_bOpenLogbook", &m_rCrippleBox.m_pInterfaceRef->m_bOpenLogbook);
    ImGui::Checkbox("m_bOpenPauseMenu", &m_rCrippleBox.m_pInterfaceRef->m_bOpenPauseMenu);
#pragma endregion
}

void ZPlayerCrippleBoxEffectBase::Stop()
{
    m_rCrippleBox.m_pInterfaceRef->Deactivate(0);
}

void ZPlayerCrippleBoxEffectBase::ActivateCrippleBox()
{
    if (const auto s_rPlayer = SDK()->GetLocalPlayer())
    {
        m_rCrippleBox.m_pInterfaceRef->m_bActivateOnStart = true;
        m_rCrippleBox.m_pInterfaceRef->m_rHitmanCharacter = s_rPlayer;

        m_rCrippleBox.m_pInterfaceRef->Activate(0);
    }
    else
    {
        m_bIsAvailable = false;
    }
}
