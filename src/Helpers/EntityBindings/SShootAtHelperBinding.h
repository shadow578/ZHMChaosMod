#pragma once
#include "Helpers/EntityBinding.h"

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/Enums.h>

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ShootAtHelper of NPCActor, introduced by patch
// NPCActor_ShootAtHelper.entity.patch.json in version 1.3.0 of Companion Mod.
struct SShootAtHelperBinding
{
	BINDING_CONSTRUCTOR(SShootAtHelperBinding);

	PROPERTY_RO(TEntityRef<ZSpatialEntity>, m_rTarget); // Target spatial to shoot at. A ray will be cast from the weapon to this spatial. Note that by default, the spatial is parented to the actor.

	PROPERTY(bool, m_bAllowActorHitActor);     // if true, allows hitting other actors. if false, only hitman can be hit.
	PROPERTY(bool, m_bUnnoticed);		       // is the shot unnoticed?
	PROPERTY(EDeathContext, m_eDeathContext);  // death context for the hit, if actor dies.

	PROPERTY(float32, m_fMinTime);		       // in continuous fire mode, minimum time between shots.
	PROPERTY(float32, m_fMaxTime);		       // in continuous fire mode, maximum time between shots.

	INPUT_PIN(Holster);   // Holster the weapon.
	INPUT_PIN(Unholster); // Unholster the weapon.
	INPUT_PIN(Fire);      // Unholster and fire the weapon. There may be some delay before firing the shot, as unholstering takes some time.
	INPUT_PIN(StartFireContinuous); // Unholster and start firing the weapon continuously.
	INPUT_PIN(StopFireContinuous);  // Stop firing the weapon and holster it.
};
