#include "ZPlayerCrippleBoxEffectBase.h"

#include <Logging.h>

#include <Glacier/ZHM5CrippleBox.h>

#define TAG "[ZPlayerCrippleBoxEffectBase] "

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
        Logger::Warn(TAG "::ActivateCrippleBox() could not activate, failed to get player reference.");
    }
}

void ZPlayerCrippleBoxDbgEffect::OnDrawDebugUI()
{
    auto s_rCrippleBox = GetCrippleBox();
    if (!s_rCrippleBox)
    {
        ImGui::TextUnformatted("CrippleBox not spawned, cannot access flags!");
        return;
    }

    ImGui::Checkbox("Activate on Flag change", &m_bReapplyOnChange);

    ImGui::SeparatorText("Flags");

#define FLAG_CHKBOX(NAME)                                                                                                     \
    if (ImGui::Checkbox(#NAME, &s_rCrippleBox.m_pInterfaceRef->NAME) && m_bReapplyOnChange)                                   \
    {                                                                                                                         \
        ActivateCrippleBox();                                                                                                 \
        Logger::Debug("[ZPlayerCrippleBoxDbgEffect] " #NAME " = {}", s_rCrippleBox.m_pInterfaceRef->NAME ? "true" : "false"); \
    }

#pragma region Flags
    FLAG_CHKBOX(m_bInSequence);
    FLAG_CHKBOX(m_bAllowBaseMovements);
    FLAG_CHKBOX(m_bSequenceAllowCamera);
    FLAG_CHKBOX(m_bLedges);
    FLAG_CHKBOX(m_bCover);
    FLAG_CHKBOX(m_bTakeClothes);
    FLAG_CHKBOX(m_bLadder);
    FLAG_CHKBOX(m_bPullVictimFromWindow);
    FLAG_CHKBOX(m_bFiberWireKill);
    FLAG_CHKBOX(m_bClimbWindow);
    FLAG_CHKBOX(m_bThrowBodyOverRail);
    FLAG_CHKBOX(m_bDumpBody);
    FLAG_CHKBOX(m_bOperateCPDoor);
    FLAG_CHKBOX(m_bHideInCloset);
    FLAG_CHKBOX(m_bCloseCombat);
    FLAG_CHKBOX(m_bGrabVictim);
    FLAG_CHKBOX(m_bPushVictimThroughWindowAndRail);
    FLAG_CHKBOX(m_bKickVictimOverLedge);
    FLAG_CHKBOX(m_bPickupItem);
    FLAG_CHKBOX(m_bDropItem);
    FLAG_CHKBOX(m_bDragBody);
    FLAG_CHKBOX(m_bThrowItem);
    FLAG_CHKBOX(m_bPlaceItem);
    FLAG_CHKBOX(m_bInteractions);
    FLAG_CHKBOX(m_bUseDeathAnimation);
    FLAG_CHKBOX(m_bVRTurn);
    FLAG_CHKBOX(m_bVRThrow);
    FLAG_CHKBOX(m_bRun);
    FLAG_CHKBOX(m_bTurn);
    FLAG_CHKBOX(m_bSneak);
    FLAG_CHKBOX(m_bNoSnapSneak);
    FLAG_CHKBOX(m_bStandUp);
    FLAG_CHKBOX(m_bFastWalk);
    FLAG_CHKBOX(m_bMovementAllowed);
    FLAG_CHKBOX(m_bIdleAnimationsAllowed);
    FLAG_CHKBOX(m_bItems);
    FLAG_CHKBOX(m_bCanHolsterItems);
    FLAG_CHKBOX(m_bCoverTakedown);
    FLAG_CHKBOX(m_bCoverScale);
    FLAG_CHKBOX(m_bCoverToCover);
    FLAG_CHKBOX(m_bCloseCombatSnapNeck);
    FLAG_CHKBOX(m_bCloseCombatChoke);
    FLAG_CHKBOX(m_bCloseCombatPistolFinish);
    FLAG_CHKBOX(m_bCloseCombatProps);
    FLAG_CHKBOX(m_bCloseCombatStandard);
    FLAG_CHKBOX(m_bCloseCombatFakeSwing);
    FLAG_CHKBOX(m_bGameCameraAutoAlign);
    FLAG_CHKBOX(m_bCameraSide);
    FLAG_CHKBOX(m_bNotSelfieMode);
    FLAG_CHKBOX(m_bInstinct);
    FLAG_CHKBOX(m_bBlindFire);
    FLAG_CHKBOX(m_bAim);
    FLAG_CHKBOX(m_bHairTrigger);
    FLAG_CHKBOX(m_bFire);
    FLAG_CHKBOX(m_bLimitedAmmo);
    FLAG_CHKBOX(m_bOpenLogbook);
    FLAG_CHKBOX(m_bOpenPauseMenu);
#pragma endregion

#undef FLAG_CHKBOX

    ImGui::SeparatorText("ZPlayerCrippleBoxEffectBase");
    ZPlayerCrippleBoxEffectBase::OnDrawDebugUI();
}

#ifdef _DEBUG
#include "Registry.h"

REGISTER_CHAOS_EFFECT(ZPlayerCrippleBoxDbgEffect);
#endif
