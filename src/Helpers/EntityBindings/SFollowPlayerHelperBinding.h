#pragma once
#include "Helpers/EntityBinding.h"

#include "SAIModifierRoleBinding.h"

#include <Glacier/Enums.h>

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_FollowPlayerHelper of NPCActor, introduced by patch
// NPCActor_FollowPlayerHelper.entity.patch.json in version 1.3.0 of Companion Mod.
struct SFollowPlayerHelperBinding
{
	BINDING_CONSTRUCTOR(SFollowPlayerHelperBinding);

	PROPERTY_RO(ZEntityRef, m_rAIModifierRole); // Reference to ZAIModifierRole sub-entity
	MEMBER_BINDING(SAIModifierRoleBinding, m_AIModifierRoleBinding, m_rAIModifierRole);

	PROPERTY(float32, m_fMinTetherRange);  // Minimum distance to the player before the stops walking
	PROPERTY(float32, m_fMaxTetherRange);  // Maximum distance to the player before the actor starts moving to catch up

	INPUT_PIN(StartFollowHitman); // Start following the player
	INPUT_PIN(StopFollowHitman);  // Stop following the player
};
