#include "ZSpeakEntityEffectBase.h"

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ActorUtils.h"

#pragma region Debug Combo Maps
// list of all EActorSoundDefs values and their string names
static const std::map<EActorSoundDefs, std::string> g_mSoundDefs = {
		{EActorSoundDefs::_NoSound, "_NoSound"},
		{EActorSoundDefs::Dth_BrkNck, "Dth_BrkNck"},
		{EActorSoundDefs::Dth_Fll, "Dth_Fll"},
		{EActorSoundDefs::Dth_GnSht, "Dth_GnSht"},
		{EActorSoundDefs::Dth_HdSht, "Dth_HdSht"},
		{EActorSoundDefs::Dth_Mpct, "Dth_Mpct"},
		{EActorSoundDefs::Dth_SltThrt, "Dth_SltThrt"},
		{EActorSoundDefs::Dth_Strngl, "Dth_Strngl"},
		{EActorSoundDefs::Dth_Xplo, "Dth_Xplo"},
		{EActorSoundDefs::Dth_PrpF, "Dth_PrpF"},
		{EActorSoundDefs::Dth_Electro, "Dth_Electro"},
		{EActorSoundDefs::Dth_Burn, "Dth_Burn"},
		{EActorSoundDefs::Dth_Crush, "Dth_Crush"},
		{EActorSoundDefs::Dth_Scrm, "Dth_Scrm"},
		{EActorSoundDefs::Dth_Hrt, "Dth_Hrt"},
		{EActorSoundDefs::Dth_SrpsGrab, "Dth_SrpsGrab"},
		{EActorSoundDefs::Dth_HumShldStrain, "Dth_HumShldStrain"},
		{EActorSoundDefs::Dth_Snore, "Dth_Snore"},
		{EActorSoundDefs::Dth_Groan, "Dth_Groan"},
		{EActorSoundDefs::Dth_Dump, "Dth_Dump"},
		{EActorSoundDefs::Dth_PrpTssFrntAck, "Dth_PrpTssFrntAck"},
		{EActorSoundDefs::Dth_Headlock, "Dth_Headlock"},
		{EActorSoundDefs::Dth_Blinded, "Dth_Blinded"},
		{EActorSoundDefs::Dth_BeeSting, "Dth_BeeSting"},
		{EActorSoundDefs::Dth_Grab, "Dth_Grab"},
		{EActorSoundDefs::Gen_Grt47, "Gen_Grt47"},
		{EActorSoundDefs::Gen_GrtGrd47WGun, "Gen_GrtGrd47WGun"},
		{EActorSoundDefs::Gen_GrtTrgt, "Gen_GrtTrgt"},
		{EActorSoundDefs::Gen_GrtTrgtRsp, "Gen_GrtTrgtRsp"},
		{EActorSoundDefs::Gen_NPC2NPCGrt, "Gen_NPC2NPCGrt"},
		{EActorSoundDefs::Gen_NPC2NPCRsp, "Gen_NPC2NPCRsp"},
		{EActorSoundDefs::Gen_GtHlp, "Gen_GtHlp"},
		{EActorSoundDefs::Gen_GtHlpLd, "Gen_GtHlpLd"},
		{EActorSoundDefs::Gen_GtHlp47Knwn, "Gen_GtHlp47Knwn"},
		{EActorSoundDefs::Gen_Mssng, "Gen_Mssng"},
		{EActorSoundDefs::Gen_HMHere, "Gen_HMHere"},
		{EActorSoundDefs::Gen_HMThere, "Gen_HMThere"},
		{EActorSoundDefs::Gen_SrpsLow, "Gen_SrpsLow"},
		{EActorSoundDefs::Gen_SrpsLowShort, "Gen_SrpsLowShort"},
		{EActorSoundDefs::Gen_Srps, "Gen_Srps"},
		{EActorSoundDefs::Gen_SrpsLd, "Gen_SrpsLd"},
		{EActorSoundDefs::Gen_StndRsp, "Gen_StndRsp"},
		{EActorSoundDefs::Gen_Stop, "Gen_Stop"},
		{EActorSoundDefs::Gen_StopLd, "Gen_StopLd"},
		{EActorSoundDefs::Gen_Reveal, "Gen_Reveal"},
		{EActorSoundDefs::Gen_ThumbsUp, "Gen_ThumbsUp"},
		{EActorSoundDefs::Gen_BrknAck, "Gen_BrknAck"},
		{EActorSoundDefs::Gen_Ack, "Gen_Ack"},
		{EActorSoundDefs::Gen_AckLd, "Gen_AckLd"},
		{EActorSoundDefs::Gen_AckNtnse, "Gen_AckNtnse"},
		{EActorSoundDefs::Gen_BumpAck, "Gen_BumpAck"},
		{EActorSoundDefs::Gen_Curse, "Gen_Curse"},
		{EActorSoundDefs::Gen_CurseLow, "Gen_CurseLow"},
		{EActorSoundDefs::Gen_DrpGun, "Gen_DrpGun"},
		{EActorSoundDefs::Gen_DrpCase, "Gen_DrpCase"},
		{EActorSoundDefs::Gen_CoinCurse, "Gen_CoinCurse"},
		{EActorSoundDefs::Gen_TransportGreet, "Gen_TransportGreet"},
		{EActorSoundDefs::Gen_Thanks, "Gen_Thanks"},
		{EActorSoundDefs::Gen_ReturnItem2Guard, "Gen_ReturnItem2Guard"},
		{EActorSoundDefs::Gen_NoWay1, "Gen_NoWay1"},
		{EActorSoundDefs::Gen_NoWay2Kidding, "Gen_NoWay2Kidding"},
		{EActorSoundDefs::Gen_NoWay3Joke, "Gen_NoWay3Joke"},
		{EActorSoundDefs::Gen_NoWay44Real, "Gen_NoWay44Real"},
		{EActorSoundDefs::Gen_NoWay5DntBeliv, "Gen_NoWay5DntBeliv"},
		{EActorSoundDefs::Gen_NoWay6Serious, "Gen_NoWay6Serious"},
		{EActorSoundDefs::Gen_NoWay7Horrible, "Gen_NoWay7Horrible"},
		{EActorSoundDefs::Gen_Way1, "Gen_Way1"},
		{EActorSoundDefs::Gen_Way2Kidding, "Gen_Way2Kidding"},
		{EActorSoundDefs::Gen_Way3Joke, "Gen_Way3Joke"},
		{EActorSoundDefs::Gen_Way44Real, "Gen_Way44Real"},
		{EActorSoundDefs::Gen_Way5DntBeliv, "Gen_Way5DntBeliv"},
		{EActorSoundDefs::Gen_Way6Serious, "Gen_Way6Serious"},
		{EActorSoundDefs::Gen_Way7Horrible, "Gen_Way7Horrible"},
		{EActorSoundDefs::Gen_NkdRunAck, "Gen_NkdRunAck"},
		{EActorSoundDefs::Gen_Grasp, "Gen_Grasp"},
		{EActorSoundDefs::Gen_Amused, "Gen_Amused"},
		{EActorSoundDefs::Gen_Annoyed, "Gen_Annoyed"},
		{EActorSoundDefs::Gen_BdygrdArrive, "Gen_BdygrdArrive"},
		{EActorSoundDefs::Gen_BdygrdMovOut, "Gen_BdygrdMovOut"},
		{EActorSoundDefs::Gen_GiveUp, "Gen_GiveUp"},
		{EActorSoundDefs::Gen_Off, "Gen_Off"},
		{EActorSoundDefs::Gen_On, "Gen_On"},
		{EActorSoundDefs::Gen_PanicLow, "Gen_PanicLow"},
		{EActorSoundDefs::Gen_Sick, "Gen_Sick"},
		{EActorSoundDefs::Gen_SmellAck, "Gen_SmellAck"},
		{EActorSoundDefs::Gen_SmrtPhnAct, "Gen_SmrtPhnAct"},
		{EActorSoundDefs::Gen_PhoneAct, "Gen_PhoneAct"},
		{EActorSoundDefs::Gen_OutbreakInfect, "Gen_OutbreakInfect"},
		{EActorSoundDefs::Gen_OutbreakSick, "Gen_OutbreakSick"},
		{EActorSoundDefs::Gen_OutbreakWhine, "Gen_OutbreakWhine"},
		{EActorSoundDefs::Gtag, "Gtag"},
		{EActorSoundDefs::ClsCmbt_Ack, "ClsCmbt_Ack"},
		{EActorSoundDefs::ClsCmbt_Tnt, "ClsCmbt_Tnt"},
		{EActorSoundDefs::Cmbt_BackupCll, "Cmbt_BackupCll"},
		{EActorSoundDefs::Cmbt_BadDsg, "Cmbt_BadDsg"},
		{EActorSoundDefs::Cmbt_Beg, "Cmbt_Beg"},
		{EActorSoundDefs::Cmbt_ClsAck, "Cmbt_ClsAck"},
		{EActorSoundDefs::Cmbt_Fire, "Cmbt_Fire"},
		{EActorSoundDefs::Cmbt_FireLdr, "Cmbt_FireLdr"},
		{EActorSoundDefs::Cmbt_GtHit, "Cmbt_GtHit"},
		{EActorSoundDefs::Cmbt_HitHM, "Cmbt_HitHM"},
		{EActorSoundDefs::Cmbt_HMClsCmbtAck, "Cmbt_HMClsCmbtAck"},
		{EActorSoundDefs::Cmbt_HMCvr, "Cmbt_HMCvr"},
		{EActorSoundDefs::Cmbt_HMFire, "Cmbt_HMFire"},
		{EActorSoundDefs::Cmbt_HMFlnk, "Cmbt_HMFlnk"},
		{EActorSoundDefs::Cmbt_HMHeadPopr, "Cmbt_HMHeadPopr"},
		{EActorSoundDefs::Cmbt_HMKll, "Cmbt_HMKll"},
		{EActorSoundDefs::Cmbt_HMKllCiv, "Cmbt_HMKllCiv"},
		{EActorSoundDefs::Cmbt_HMKllName, "Cmbt_HMKllName"},
		{EActorSoundDefs::Cmbt_HMKllPrpTss, "Cmbt_HMKllPrpTss"},
		{EActorSoundDefs::Cmbt_HMMssTnt, "Cmbt_HMMssTnt"},
		{EActorSoundDefs::Cmbt_HMShrpShtr, "Cmbt_HMShrpShtr"},
		{EActorSoundDefs::Cmbt_HMSpttd, "Cmbt_HMSpttd"},
		{EActorSoundDefs::Cmbt_HMVnshd, "Cmbt_HMVnshd"},
		{EActorSoundDefs::Cmbt_Hold, "Cmbt_Hold"},
		{EActorSoundDefs::Cmbt_HoldLdr, "Cmbt_HoldLdr"},
		{EActorSoundDefs::Cmbt_HumShldRls1, "Cmbt_HumShldRls1"},
		{EActorSoundDefs::Cmbt_HumShldRls2, "Cmbt_HumShldRls2"},
		{EActorSoundDefs::Cmbt_HumShldRls3, "Cmbt_HumShldRls3"},
		{EActorSoundDefs::Cmbt_HumShldRlsFem1, "Cmbt_HumShldRlsFem1"},
		{EActorSoundDefs::Cmbt_HumShldRlsFem2, "Cmbt_HumShldRlsFem2"},
		{EActorSoundDefs::Cmbt_HumShldRlsFem3, "Cmbt_HumShldRlsFem3"},
		{EActorSoundDefs::Cmbt_HumShldVctm, "Cmbt_HumShldVctm"},
		{EActorSoundDefs::Cmbt_HumShldLdr, "Cmbt_HumShldLdr"},
		{EActorSoundDefs::Cmbt_LngLst, "Cmbt_LngLst"},
		{EActorSoundDefs::Cmbt_LngLstRsp, "Cmbt_LngLstRsp"},
		{EActorSoundDefs::Cmbt_LstMnStn, "Cmbt_LstMnStn"},
		{EActorSoundDefs::Cmbt_LstSght, "Cmbt_LstSght"},
		{EActorSoundDefs::Cmbt_LstSghtRsp, "Cmbt_LstSghtRsp"},
		{EActorSoundDefs::Cmbt_NdrAttck, "Cmbt_NdrAttck"},
		{EActorSoundDefs::Cmbt_Relod, "Cmbt_Relod"},
		{EActorSoundDefs::Cmbt_Scrm, "Cmbt_Scrm"},
		{EActorSoundDefs::Cmbt_ThrowFlash, "Cmbt_ThrowFlash"},
		{EActorSoundDefs::Cmbt_ThrowFlashMiss, "Cmbt_ThrowFlashMiss"},
		{EActorSoundDefs::Cmbt_ThrowFlashMiss2, "Cmbt_ThrowFlashMiss2"},
		{EActorSoundDefs::Cmbt_ThrowFlashWin, "Cmbt_ThrowFlashWin"},
		{EActorSoundDefs::Cmbt_ThrowFlashWin2, "Cmbt_ThrowFlashWin2"},
		{EActorSoundDefs::Cmbt_TkDwnLdr, "Cmbt_TkDwnLdr"},
		{EActorSoundDefs::Cmbt_VntAck, "Cmbt_VntAck"},
		{EActorSoundDefs::Cmbt_Whmp, "Cmbt_Whmp"},
		{EActorSoundDefs::Cmbt_StalemateHold, "Cmbt_StalemateHold"},
		{EActorSoundDefs::Cmbt_StalemateTnt, "Cmbt_StalemateTnt"},
		{EActorSoundDefs::Cmbt_TriggerTheAlarm, "Cmbt_TriggerTheAlarm"},
		{EActorSoundDefs::Cmbt_47Mpty, "Cmbt_47Mpty"},
		{EActorSoundDefs::Cmbt_47SuperSize, "Cmbt_47SuperSize"},
		{EActorSoundDefs::Evac_PrtTrgtSolo, "Evac_PrtTrgtSolo"},
		{EActorSoundDefs::Evac_PrtTrgtAck, "Evac_PrtTrgtAck"},
		{EActorSoundDefs::Evac_PrtTrgtAckLdr, "Evac_PrtTrgtAckLdr"},
		{EActorSoundDefs::Evac_PrtTrgtEscrt, "Evac_PrtTrgtEscrt"},
		{EActorSoundDefs::Evac_PrtTrgtStop, "Evac_PrtTrgtStop"},
		{EActorSoundDefs::Evac_PrtTrgtStnd, "Evac_PrtTrgtStnd"},
		{EActorSoundDefs::Evac_PrtTrgtStndRsp, "Evac_PrtTrgtStndRsp"},
		{EActorSoundDefs::Evac_Cornered, "Evac_Cornered"},
		{EActorSoundDefs::Evac_MovOut, "Evac_MovOut"},
		{EActorSoundDefs::Evac_PathChange, "Evac_PathChange"},
		{EActorSoundDefs::Evac_PeelOff, "Evac_PeelOff"},
		{EActorSoundDefs::Evac_LastPeelOff, "Evac_LastPeelOff"},
		{EActorSoundDefs::Evac_ShltrArrv, "Evac_ShltrArrv"},
		{EActorSoundDefs::Evac_ShltrBad, "Evac_ShltrBad"},
		{EActorSoundDefs::Evac_ShltrLdr, "Evac_ShltrLdr"},
		{EActorSoundDefs::Evac_ShltrRsp, "Evac_ShltrRsp"},
		{EActorSoundDefs::Evac_TrgtHitRsp, "Evac_TrgtHitRsp"},
		{EActorSoundDefs::AvoidXplo_Ack, "AvoidXplo_Ack"},
		{EActorSoundDefs::AvoidXplo_Stnd, "AvoidXplo_Stnd"},
		{EActorSoundDefs::Ar_47BadAction, "Ar_47BadAction"},
		{EActorSoundDefs::Ar_47X, "Ar_47X"},
		{EActorSoundDefs::Ar_BadDsg, "Ar_BadDsg"},
		{EActorSoundDefs::Ar_BlmeKll, "Ar_BlmeKll"},
		{EActorSoundDefs::Ar_BlameKnckDwn, "Ar_BlameKnckDwn"},
		{EActorSoundDefs::Ar_BlameKnckDwnPT, "Ar_BlameKnckDwnPT"},
		{EActorSoundDefs::Ar_BlameKllPT, "Ar_BlameKllPT"},
		{EActorSoundDefs::Ar_47BadActionPT, "Ar_47BadActionPT"},
		{EActorSoundDefs::Ar_DrgBody, "Ar_DrgBody"},
		{EActorSoundDefs::Ar_FkeSrrdrTnt, "Ar_FkeSrrdrTnt"},
		{EActorSoundDefs::Ar_HMDoor, "Ar_HMDoor"},
		{EActorSoundDefs::Ar_Strangle, "Ar_Strangle"},
		{EActorSoundDefs::Ar_Trspss, "Ar_Trspss"},
		{EActorSoundDefs::Ar_WeapWrn1, "Ar_WeapWrn1"},
		{EActorSoundDefs::Ar_WeapWrn2, "Ar_WeapWrn2"},
		{EActorSoundDefs::Ar_Wrn1, "Ar_Wrn1"},
		{EActorSoundDefs::Ar_Wrn2, "Ar_Wrn2"},
		{EActorSoundDefs::Ar_Wrn3, "Ar_Wrn3"},
		{EActorSoundDefs::Ar_VictimAck, "Ar_VictimAck"},
		{EActorSoundDefs::Ar_Thief, "Ar_Thief"},
		{EActorSoundDefs::Ar_Rsp, "Ar_Rsp"},
		{EActorSoundDefs::Sniper_Ack, "Sniper_Ack"},
		{EActorSoundDefs::InCa_BackupCll, "InCa_BackupCll"},
		{EActorSoundDefs::InCa_ChckCvr, "InCa_ChckCvr"},
		{EActorSoundDefs::InCa_CivRptFail, "InCa_CivRptFail"},
		{EActorSoundDefs::InCa_CivUpset, "InCa_CivUpset"},
		{EActorSoundDefs::InCa_ClstTnt, "InCa_ClstTnt"},
		{EActorSoundDefs::InCa_HMTnt, "InCa_HMTnt"},
		{EActorSoundDefs::InCa_Idle, "InCa_Idle"},
		{EActorSoundDefs::InCa_NitiateHMKnwn, "InCa_NitiateHMKnwn"},
		{EActorSoundDefs::InCa_SrchLdr, "InCa_SrchLdr"},
		{EActorSoundDefs::InCa_Stnd, "InCa_Stnd"},
		{EActorSoundDefs::InCa_StndAgtd, "InCa_StndAgtd"},
		{EActorSoundDefs::InCa_StndAgtdLdr, "InCa_StndAgtdLdr"},
		{EActorSoundDefs::InCa_StndAgtdHMKnwn, "InCa_StndAgtdHMKnwn"},
		{EActorSoundDefs::InCa_StndAgtdHMKnwnLdr, "InCa_StndAgtdHMKnwnLdr"},
		{EActorSoundDefs::InCa_StndHMKnwn, "InCa_StndHMKnwn"},
		{EActorSoundDefs::InCa_StndHMKnwnLdr, "InCa_StndHMKnwnLdr"},
		{EActorSoundDefs::InCa_StndLdr, "InCa_StndLdr"},
		{EActorSoundDefs::InCa_StndRsp, "InCa_StndRsp"},
		{EActorSoundDefs::InCa_StndLckDwnFlsAlrm, "InCa_StndLckDwnFlsAlrm"},
		{EActorSoundDefs::InCa_VntTnt, "InCa_VntTnt"},
		{EActorSoundDefs::InCa_Brk2Civ, "InCa_Brk2Civ"},
		{EActorSoundDefs::InCa_Brk2Grd, "InCa_Brk2Grd"},
		{EActorSoundDefs::InCa_Brk2Rdo, "InCa_Brk2Rdo"},
		{EActorSoundDefs::InCa_BrkAsk, "InCa_BrkAsk"},
		{EActorSoundDefs::InCa_GhostAsk, "InCa_GhostAsk"},
		{EActorSoundDefs::InCa_TriggerTheAlarm, "InCa_TriggerTheAlarm"},
		{EActorSoundDefs::InCa_Xpln47Thief, "InCa_Xpln47Thief"},
		{EActorSoundDefs::InCa_DstrssInv, "InCa_DstrssInv"},
		{EActorSoundDefs::InCa_DstrssLdr, "InCa_DstrssLdr"},
		{EActorSoundDefs::InCa_DstrssInvLdr, "InCa_DstrssInvLdr"},
		{EActorSoundDefs::InCa_WakeAsk, "InCa_WakeAsk"},
		{EActorSoundDefs::InCa_47Rcall, "InCa_47Rcall"},
		{EActorSoundDefs::InCa_WakerStnd, "InCa_WakerStnd"},
		{EActorSoundDefs::InCa_ClsCmbtAck, "InCa_ClsCmbtAck"},
		{EActorSoundDefs::InCa_SeeDthInv, "InCa_SeeDthInv"},
		{EActorSoundDefs::InCa_SeeDthInvLdr, "InCa_SeeDthInvLdr"},
		{EActorSoundDefs::InCa_SeeDthLdr, "InCa_SeeDthLdr"},
		{EActorSoundDefs::InCa_XploInv, "InCa_XploInv"},
		{EActorSoundDefs::InCa_XploInvLdr, "InCa_XploInvLdr"},
		{EActorSoundDefs::InCa_XploLdr, "InCa_XploLdr"},
		{EActorSoundDefs::InCa_AlarmAck, "InCa_AlarmAck"},
		{EActorSoundDefs::InCa_GnShtInv, "InCa_GnShtInv"},
		{EActorSoundDefs::InCa_GnShtInvLdr, "InCa_GnShtInvLdr"},
		{EActorSoundDefs::InCa_GnShtLdr, "InCa_GnShtLdr"},
		{EActorSoundDefs::InCa_RecurSvrInv, "InCa_RecurSvrInv"},
		{EActorSoundDefs::InCa_RecurSvrInvLdr, "InCa_RecurSvrInvLdr"},
		{EActorSoundDefs::InCa_RecurSvrInvRsp, "InCa_RecurSvrInvRsp"},
		{EActorSoundDefs::InCa_RecurSvrLdr, "InCa_RecurSvrLdr"},
		{EActorSoundDefs::InCa_RecurSvrRsp, "InCa_RecurSvrRsp"},
		{EActorSoundDefs::InCa_LckDwnGtOutLdr, "InCa_LckDwnGtOutLdr"},
		{EActorSoundDefs::InCa_LckDwnGtOutRsp, "InCa_LckDwnGtOutRsp"},
		{EActorSoundDefs::InCa_LckDwnWrn1, "InCa_LckDwnWrn1"},
		{EActorSoundDefs::InCa_LckDwnWrn2, "InCa_LckDwnWrn2"},
		{EActorSoundDefs::InCa_LckDwnWrn3, "InCa_LckDwnWrn3"},
		{EActorSoundDefs::InCa_LckDwnCivCmnt, "InCa_LckDwnCivCmnt"},
		{EActorSoundDefs::InCa_FrskAck, "InCa_FrskAck"},
		{EActorSoundDefs::InCa_Frsk, "InCa_Frsk"},
		{EActorSoundDefs::InCa_FrskCln, "InCa_FrskCln"},
		{EActorSoundDefs::InCa_FrskWpn, "InCa_FrskWpn"},
		{EActorSoundDefs::InCa_Xpln47Wpn, "InCa_Xpln47Wpn"},
		{EActorSoundDefs::InCa_XplnAccdnt, "InCa_XplnAccdnt"},
		{EActorSoundDefs::InCa_XplnDedBdy, "InCa_XplnDedBdy"},
		{EActorSoundDefs::InCa_XplnDedBdyMassive, "InCa_XplnDedBdyMassive"},
		{EActorSoundDefs::InCa_XplnDrgBdy, "InCa_XplnDrgBdy"},
		{EActorSoundDefs::InCa_XplnDstrss, "InCa_XplnDstrss"},
		{EActorSoundDefs::InCa_XplnExplo, "InCa_XplnExplo"},
		{EActorSoundDefs::InCa_XplnGhost, "InCa_XplnGhost"},
		{EActorSoundDefs::InCa_XplnGnsht, "InCa_XplnGnsht"},
		{EActorSoundDefs::InCa_XplnNkdBdy, "InCa_XplnNkdBdy"},
		{EActorSoundDefs::InCa_XplnPcfdBdy, "InCa_XplnPcfdBdy"},
		{EActorSoundDefs::InCa_XplnSeeDth, "InCa_XplnSeeDth"},
		{EActorSoundDefs::InCa_XplnTrspss, "InCa_XplnTrspss"},
		{EActorSoundDefs::InCa_XplnX, "InCa_XplnX"},
		{EActorSoundDefs::InCa_XplnWpn, "InCa_XplnWpn"},
		{EActorSoundDefs::InCa_XplnDsg, "InCa_XplnDsg"},
		{EActorSoundDefs::InCa_XplnImposter, "InCa_XplnImposter"},
		{EActorSoundDefs::InCa_XplnRecurSvr, "InCa_XplnRecurSvr"},
		{EActorSoundDefs::InCa_XplnRsp, "InCa_XplnRsp"},
		{EActorSoundDefs::InCa_XplnAckRdo, "InCa_XplnAckRdo"},
		{EActorSoundDefs::InCa_XplnKnckDwn, "InCa_XplnKnckDwn"},
		{EActorSoundDefs::InCa_XplnKnckDwnVctm, "InCa_XplnKnckDwnVctm"},
		{EActorSoundDefs::InCa_XplnKnckDwnGhost, "InCa_XplnKnckDwnGhost"},
		{EActorSoundDefs::InCa_XplnSeeStrngl, "InCa_XplnSeeStrngl"},
		{EActorSoundDefs::InCa_XplnHuntTargetWin, "InCa_XplnHuntTargetWin"},
		{EActorSoundDefs::InCa_XplnHuntTargetFail, "InCa_XplnHuntTargetFail"},
		{EActorSoundDefs::InCa_VIPDownAck, "InCa_VIPDownAck"},
		{EActorSoundDefs::InCa_VIPKillAck, "InCa_VIPKillAck"},
		{EActorSoundDefs::Accdnt_Inv, "Accdnt_Inv"},
		{EActorSoundDefs::InDedBdy_BloodPllAck, "InDedBdy_BloodPllAck"},
		{EActorSoundDefs::InDedBdy_Ack, "InDedBdy_Ack"},
		{EActorSoundDefs::InDedBdy_NkdAck, "InDedBdy_NkdAck"},
		{EActorSoundDefs::InDedBdy_Inv, "InDedBdy_Inv"},
		{EActorSoundDefs::InDedBdy_BllPllRpt, "InDedBdy_BllPllRpt"},
		{EActorSoundDefs::InDedBdy_Massive, "InDedBdy_Massive"},
		{EActorSoundDefs::InDedBdy_PcfdInv, "InDedBdy_PcfdInv"},
		{EActorSoundDefs::InDedBdy_CntnAck, "InDedBdy_CntnAck"},
		{EActorSoundDefs::InDedBdy_Stnd, "InDedBdy_Stnd"},
		{EActorSoundDefs::InDedBdy_CircleBdy, "InDedBdy_CircleBdy"},
		{EActorSoundDefs::InDedBdy_CivCmnt, "InDedBdy_CivCmnt"},
		{EActorSoundDefs::InDedBdy_PrmtrBrchWrn1, "InDedBdy_PrmtrBrchWrn1"},
		{EActorSoundDefs::InDedBdy_PrmtrBrchWrn2, "InDedBdy_PrmtrBrchWrn2"},
		{EActorSoundDefs::InDedBdy_47AsGrdAck, "InDedBdy_47AsGrdAck"},
		{EActorSoundDefs::InDedBdy_BodyGone, "InDedBdy_BodyGone"},
		{EActorSoundDefs::InDedBdy_VctmRcvr, "InDedBdy_VctmRcvr"},
		{EActorSoundDefs::InDedBdy_WakerWake, "InDedBdy_WakerWake"},
		{EActorSoundDefs::InDedBdy_WakeRsp, "InDedBdy_WakeRsp"},
		{EActorSoundDefs::InDedBdy_WakeNkdLdr, "InDedBdy_WakeNkdLdr"},
		{EActorSoundDefs::InDedBdy_WakeNkdRsp, "InDedBdy_WakeNkdRsp"},
		{EActorSoundDefs::Rcvr_Xpln47, "Rcvr_Xpln47"},
		{EActorSoundDefs::Rcvr_XplnDsg, "Rcvr_XplnDsg"},
		{EActorSoundDefs::Rcvr_XplnKnckDwn, "Rcvr_XplnKnckDwn"},
		{EActorSoundDefs::InDsg_FllwWrn1, "InDsg_FllwWrn1"},
		{EActorSoundDefs::InDsg_FllwWrn2, "InDsg_FllwWrn2"},
		{EActorSoundDefs::InDsg_FllwWrn3, "InDsg_FllwWrn3"},
		{EActorSoundDefs::InDsg_Pzzl, "InDsg_Pzzl"},
		{EActorSoundDefs::InDsg_Stnd, "InDsg_Stnd"},
		{EActorSoundDefs::InDsg_StndDistance, "InDsg_StndDistance"},
		{EActorSoundDefs::InDsg_StndHidden, "InDsg_StndHidden"},
		{EActorSoundDefs::InDsg_HdNPlnSght, "InDsg_HdNPlnSght"},
		{EActorSoundDefs::InDsg_FllwWrn1Ack, "InDsg_FllwWrn1Ack"},
		{EActorSoundDefs::InDsg_FllwWrn1BadAction, "InDsg_FllwWrn1BadAction"},
		{EActorSoundDefs::InDsg_FllwWrn1Wpn, "InDsg_FllwWrn1Wpn"},
		{EActorSoundDefs::InDsg_FllwWrn1BadSound, "InDsg_FllwWrn1BadSound"},
		{EActorSoundDefs::InDsg_FllwWrnJoinr, "InDsg_FllwWrnJoinr"},
		{EActorSoundDefs::InDsg_FllwWrn1ShadyItem, "InDsg_FllwWrn1ShadyItem"},
		{EActorSoundDefs::Trspss_Stnd, "Trspss_Stnd"},
		{EActorSoundDefs::Trspss_Wrn1, "Trspss_Wrn1"},
		{EActorSoundDefs::Trspss_Wrn2, "Trspss_Wrn2"},
		{EActorSoundDefs::Trspss_Wrn3, "Trspss_Wrn3"},
		{EActorSoundDefs::Trspss_Rsp, "Trspss_Rsp"},
		{EActorSoundDefs::Trspss_SrchAckLegal47, "Trspss_SrchAckLegal47"},
		{EActorSoundDefs::Trspss_EscortAck, "Trspss_EscortAck"},
		{EActorSoundDefs::Trspss_EscortRequest, "Trspss_EscortRequest"},
		{EActorSoundDefs::Trspss_EscortRequestRepeat, "Trspss_EscortRequestRepeat"},
		{EActorSoundDefs::Trspss_EscortStayClose, "Trspss_EscortStayClose"},
		{EActorSoundDefs::Trspss_EscortOk, "Trspss_EscortOk"},
		{EActorSoundDefs::Trspss_EscortStnd, "Trspss_EscortStnd"},
		{EActorSoundDefs::Trspss_EscortArrest, "Trspss_EscortArrest"},
		{EActorSoundDefs::Trspss_EscortExit, "Trspss_EscortExit"},
		{EActorSoundDefs::Trspss_EscortStayRequest, "Trspss_EscortStayRequest"},
		{EActorSoundDefs::InCu_Brk2Rdo, "InCu_Brk2Rdo"},
		{EActorSoundDefs::InCu_CivCmnd, "InCu_CivCmnd"},
		{EActorSoundDefs::InCu_Stnd, "InCu_Stnd"},
		{EActorSoundDefs::InCu_CivRsp, "InCu_CivRsp"},
		{EActorSoundDefs::InCu_BackupRqst, "InCu_BackupRqst"},
		{EActorSoundDefs::InCu_CrAlrmAck, "InCu_CrAlrmAck"},
		{EActorSoundDefs::InCu_CrAlrmLdr, "InCu_CrAlrmLdr"},
		{EActorSoundDefs::InCu_CrAlrmStndRsp, "InCu_CrAlrmStndRsp"},
		{EActorSoundDefs::InCu_FtStpsAck, "InCu_FtStpsAck"},
		{EActorSoundDefs::InCu_FtStpsStnd, "InCu_FtStpsStnd"},
		{EActorSoundDefs::InCu_PrpTssHearAck, "InCu_PrpTssHearAck"},
		{EActorSoundDefs::InCu_PrpTssHearInv, "InCu_PrpTssHearInv"},
		{EActorSoundDefs::InCu_PrpTssHearLdr, "InCu_PrpTssHearLdr"},
		{EActorSoundDefs::InCu_PrpTssHearStndRsp, "InCu_PrpTssHearStndRsp"},
		{EActorSoundDefs::InCu_PrpTssSeeAck, "InCu_PrpTssSeeAck"},
		{EActorSoundDefs::InCu_PrpTssSeeInv, "InCu_PrpTssSeeInv"},
		{EActorSoundDefs::InCu_PrpTssSeeLdr, "InCu_PrpTssSeeLdr"},
		{EActorSoundDefs::InCu_PrpTssSeeStndRsp, "InCu_PrpTssSeeStndRsp"},
		{EActorSoundDefs::InCu_RdoAck, "InCu_RdoAck"},
		{EActorSoundDefs::InCu_RdoInv, "InCu_RdoInv"},
		{EActorSoundDefs::InCu_RdoLdr, "InCu_RdoLdr"},
		{EActorSoundDefs::InCu_RdoStndRsp, "InCu_RdoStndRsp"},
		{EActorSoundDefs::InCu_WpnInv, "InCu_WpnInv"},
		{EActorSoundDefs::InCu_RecurAck, "InCu_RecurAck"},
		{EActorSoundDefs::InCu_RecurInv, "InCu_RecurInv"},
		{EActorSoundDefs::InCu_RecurLdr, "InCu_RecurLdr"},
		{EActorSoundDefs::InCu_RecurRsp, "InCu_RecurRsp"},
		{EActorSoundDefs::InCu_ItemAckLdr, "InCu_ItemAckLdr"},
		{EActorSoundDefs::InCu_CrAlrmStndStndRsp, "InCu_CrAlrmStndStndRsp"},
		{EActorSoundDefs::InCu_EscrtTrgtRedLight, "InCu_EscrtTrgtRedLight"},
		{EActorSoundDefs::InCu_EscrtTrgtGreenLight, "InCu_EscrtTrgtGreenLight"},
		{EActorSoundDefs::InSt_HMAglty, "InSt_HMAglty"},
		{EActorSoundDefs::InSt_HMBz, "InSt_HMBz"},
		{EActorSoundDefs::InSt_HMBzStnd, "InSt_HMBzStnd"},
		{EActorSoundDefs::InSt_HMEntXit, "InSt_HMEntXit"},
		{EActorSoundDefs::InSt_HMInCvr, "InSt_HMInCvr"},
		{EActorSoundDefs::InSt_HMSnkng, "InSt_HMSnkng"},
		{EActorSoundDefs::InSt_PrpTssSee, "InSt_PrpTssSee"},
		{EActorSoundDefs::InSt_Stnd, "InSt_Stnd"},
		{EActorSoundDefs::InSt_Wrn, "InSt_Wrn"},
		{EActorSoundDefs::InSt_HM2Cls, "InSt_HM2Cls"},
		{EActorSoundDefs::InSt_SickAck, "InSt_SickAck"},
		{EActorSoundDefs::InSt_AdiosRequest, "InSt_AdiosRequest"},
		{EActorSoundDefs::InSt_PQ, "InSt_PQ"},
		{EActorSoundDefs::FseBx_Fixed, "FseBx_Fixed"},
		{EActorSoundDefs::FseBx_Fixing, "FseBx_Fixing"},
		{EActorSoundDefs::FseBx_GoFix, "FseBx_GoFix"},
		{EActorSoundDefs::FseBx_SabAck, "FseBx_SabAck"},
		{EActorSoundDefs::Sentry_DenyEntry, "Sentry_DenyEntry"},
		{EActorSoundDefs::Sentry_Frsk, "Sentry_Frsk"},
		{EActorSoundDefs::Sentry_FrskRequest, "Sentry_FrskRequest"},
		{EActorSoundDefs::Sentry_ItemRequest, "Sentry_ItemRequest"},
		{EActorSoundDefs::Sentry_Accepted, "Sentry_Accepted"},
		{EActorSoundDefs::Sentry_FrskWpnAck, "Sentry_FrskWpnAck"},
		{EActorSoundDefs::Sentry_47LoiterAck, "Sentry_47LoiterAck"},
		{EActorSoundDefs::Sentry_DenyDsg, "Sentry_DenyDsg"},
		{EActorSoundDefs::Sentry_PostCommentLdr, "Sentry_PostCommentLdr"},
		{EActorSoundDefs::Sentry_PostCommentRsp, "Sentry_PostCommentRsp"},
		{EActorSoundDefs::VIP_MssgnA_Ldr, "VIP_MssgnA_Ldr"},
		{EActorSoundDefs::VIP_MssgnB_Rsp, "VIP_MssgnB_Rsp"},
		{EActorSoundDefs::VIP_MssgnC_Ldr, "VIP_MssgnC_Ldr"},
		{EActorSoundDefs::VIP_MssgnD_Rsp, "VIP_MssgnD_Rsp"},
		{EActorSoundDefs::VIP_MssngCallOut, "VIP_MssngCallOut"},
		{EActorSoundDefs::Dth_Sick, "Dth_Sick"},
		{EActorSoundDefs::Dth_Poison, "Dth_Poison"},
		{EActorSoundDefs::Gen_Avoid, "Gen_Avoid"},
		{EActorSoundDefs::Gen_CloseCall, "Gen_CloseCall"},
		{EActorSoundDefs::Gen_PhnPckUP, "Gen_PhnPckUP"},
		{EActorSoundDefs::Gen_PhoneActLockdown, "Gen_PhoneActLockdown"},
		{EActorSoundDefs::Cmbt_FlushOutLdr, "Cmbt_FlushOutLdr"},
		{EActorSoundDefs::Cmbt_HMPrptssKnckOut, "Cmbt_HMPrptssKnckOut"},
		{EActorSoundDefs::InCa_FrskHeadsUpLdr, "InCa_FrskHeadsUpLdr"},
		{EActorSoundDefs::InCa_FrskHeadsUpRdo, "InCa_FrskHeadsUpRdo"},
		{EActorSoundDefs::InCa_XplnLOS, "InCa_XplnLOS"},
		{EActorSoundDefs::InCa_XplnGotShot, "InCa_XplnGotShot"},
		{EActorSoundDefs::InDedBdy_CivCmntPhone, "InDedBdy_CivCmntPhone"},
		{EActorSoundDefs::InDedBdy_NoFaulVctmXpln, "InDedBdy_NoFaulVctmXpln"},
		{EActorSoundDefs::InDsg_FllwWrn1Nkd, "InDsg_FllwWrn1Nkd"},
		{EActorSoundDefs::Ar_BlameKnckDwnMelee, "Ar_BlameKnckDwnMelee"},
		{EActorSoundDefs::Exp_Carry, "Exp_Carry"},
		{EActorSoundDefs::Exp_ClearThroat, "Exp_ClearThroat"},
		{EActorSoundDefs::Exp_Cough, "Exp_Cough"},
		{EActorSoundDefs::Exp_Drink, "Exp_Drink"},
		{EActorSoundDefs::Exp_Exhale, "Exp_Exhale"},
		{EActorSoundDefs::Exp_Idle, "Exp_Idle"},
		{EActorSoundDefs::Exp_Inhale, "Exp_Inhale"},
		{EActorSoundDefs::Exp_InhaleFast, "Exp_InhaleFast"},
		{EActorSoundDefs::Exp_Sniff, "Exp_Sniff"},
		{EActorSoundDefs::Exp_Swallow, "Exp_Swallow"},
		{EActorSoundDefs::Exp_Think, "Exp_Think"},
		{EActorSoundDefs::Exp_Scared, "Exp_Scared"},
		{EActorSoundDefs::Exp_Gld, "Exp_Gld"},
		{EActorSoundDefs::Exp_Dsppntd, "Exp_Dsppntd"},
		{EActorSoundDefs::Exp_InPain, "Exp_InPain"},
		{EActorSoundDefs::InCa_AckBdy, "InCa_AckBdy"},
		{EActorSoundDefs::InCa_AckBdyLdr, "InCa_AckBdyLdr"},
		{EActorSoundDefs::InDedBdy_CivCmntPcfd, "InDedBdy_CivCmntPcfd"},
		{EActorSoundDefs::InDedBdy_CivCmntPhonePcfd, "InDedBdy_CivCmntPhonePcfd"},
		{EActorSoundDefs::Gen_SocialAck, "Gen_SocialAck"}};

// list of all EGestureCategory values and their string names
static const std::map<EGestureCategory, std::string> g_mGestureCategories = {
		{EGestureCategory::EGC_None, "EGC_None"},
		{EGestureCategory::EGC_AmbientDialog, "EGC_AmbientDialog"},
		{EGestureCategory::EGC_TrespassWarn1, "EGC_TrespassWarn1"},
		{EGestureCategory::EGC_TrespassWarn2, "EGC_TrespassWarn2"},
		{EGestureCategory::EGC_TrespassWarn3, "EGC_TrespassWarn3"},
		{EGestureCategory::EGC_StopWarn, "EGC_StopWarn"}};
#pragma endregion

void ZSpeakEntityEffectBase::LoadResources()
{
	m_pSpeakEntitySpawner = ZTemplateEntitySpawner::Create<"[modules:/zspeakentity_sounddef.class].pc_entitytype">();
}

void ZSpeakEntityEffectBase::OnClearScene()
{
	m_pSpeakEntitySpawner = nullptr;
}

bool ZSpeakEntityEffectBase::Available() const
{
	return IChaosEffect::Available() &&
				 m_pSpeakEntitySpawner && m_pSpeakEntitySpawner->IsAvailable();
}

void ZSpeakEntityEffectBase::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("Prop: {}", m_pSpeakEntitySpawner->ToString()).c_str());

	ImGui::BeginDisabled(!Available());

	if (ImGui::BeginCombo("DBG SoundDef", g_mSoundDefs.at(m_eDebugSoundDef).c_str()))
	{
		for (auto [s_eDef, s_sName] : g_mSoundDefs)
		{
			if (ImGui::Selectable(
							s_sName.c_str(),
							s_eDef == m_eDebugSoundDef))
			{
				m_eDebugSoundDef = s_eDef;
				DebugSpeakAtNearestActor();
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("DBG Gesture Category", g_mGestureCategories.at(m_eDebugGestureCategory).c_str()))
	{
		for (auto [s_eCat, s_sName] : g_mGestureCategories)
		{
			if (ImGui::Selectable(
							s_sName.c_str(),
							s_eCat == m_eDebugGestureCategory))
			{
				m_eDebugGestureCategory = s_eCat;
				DebugSpeakAtNearestActor();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Checkbox("DBG Allow Interrupt", &m_bDebugAllowInterrupt);

	if (ImGui::Button("Speak at Nearest Actor"))
	{
		DebugSpeakAtNearestActor();
	}

	ImGui::EndDisabled();
}

void ZSpeakEntityEffectBase::DebugSpeakAtNearestActor()
{
	if (const auto s_rPlayer = SDK()->GetLocalPlayer())
	{
		if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
		{
			const auto s_vPlayerPos = s_pPlayerSpatial->GetObjectToWorldMatrix().Trans;

			if (const auto s_aNearby = Utils::GetNearbyActors(s_vPlayerPos, 1); !s_aNearby.empty())
			{
				const auto s_rActor = s_aNearby.front().first;
				Speak(s_rActor.m_entityRef, m_eDebugSoundDef, m_eDebugGestureCategory, m_bDebugAllowInterrupt);
			}
		}
	}
}

SSpeakEntitySounddefBinding ZSpeakEntityEffectBase::Speak(
		const ZEntityRef &p_rActor,
		const EActorSoundDefs p_eSoundDef,
		const EGestureCategory p_eGesture,
		const bool p_bAllowInterrupt,
		const bool p_bStartNow)
{
	auto s_SpeakEntity = Spawn();

	s_SpeakEntity.m_rActor = p_rActor;
	s_SpeakEntity.m_eSoundDef = p_eSoundDef;
	s_SpeakEntity.m_eGesture = p_eGesture;
	s_SpeakEntity.m_bAllowInterrupt = p_bAllowInterrupt;

	if (p_bStartNow)
	{
		s_SpeakEntity.Start();
	}

	return s_SpeakEntity;
}

SSpeakEntitySounddefBinding ZSpeakEntityEffectBase::Spawn()
{
	return SSpeakEntitySounddefBinding(m_pSpeakEntitySpawner->Spawn());
}
