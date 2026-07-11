#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include "SAIModifierBinding.h"
#include "Entity/BehaviorTreeMatch.h"

#include <Glacier/Enums.h>

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_FollowPlayerHelper of NPCActor, introduced by patch
// NPCActor_FollowPlayerHelper.entity.patch.json in version 1.3.0 of Companion Mod.
struct SFollowPlayerHelperBinding : SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY_RO(ZEntityRef, m_rAIModifier); // Reference to ZAIModifier sub-entity
    MEMBER_BINDING(SAIModifierBinding, m_AIModifierBinding, m_rAIModifier);

    PROPERTY(float32, m_fMinTetherRange);   // Minimum distance to the player before the stops walking
    PROPERTY(float32, m_fMaxTetherRange);   // Maximum distance to the player before the actor starts moving to catch up
    PROPERTY(float32, m_fMaxSightDistance); // Maximum distance at which the actor can see the player and start following
    PROPERTY(EMoveSpeed, m_eMaxMoveSpeed);  // Maximum speed at which the actor will move when following the player. when nearby, the actor will slow down to regular walking speed
    PROPERTY(ZString, m_sMatch);            // Behavior tree node name to match. Default (and lowest) ist "BT_AMBIENCE"; use "BT_OVERRIDE_ALL" to make actor ignore every other behavior. See BehaviorTreeMatch.h

    INPUT_PIN(StartFollowHitman); // Start following the player
    INPUT_PIN(StopFollowHitman);  // Stop following the player

    // Start following the player, ignoring everything else (including illegal actions, distractions, etc.).
    // After Stopping, the actor will have no memory of what happened during the follow.
    // Must stop using StopFollowHitmanIgnoreEverything, otherwise the actor will be permanently stuck in follow mode.
    inline void StartFollowHitmanIgnoreEverything()
    {
        if (auto s_AIModifier = m_AIModifierBinding; s_AIModifier)
        {
            s_AIModifier.m_bIgnoreDistractions = true;
            s_AIModifier.m_bIgnoreAccidents = true;
            s_AIModifier.m_bIgnoreDeadBodies = true;
            s_AIModifier.m_bDisableDeadBodySensor = true;
            s_AIModifier.m_bIgnoreWeapons = true;
            s_AIModifier.m_bIgnoreTrespassing = true;
            s_AIModifier.m_bDeafAndBlind = true;
        }

        m_sMatch = BehaviorTreeMatch::BT_OVERRIDE_ALL;
        StartFollowHitman();
    }

    inline void StopFollowHitmanIgnoreEverything()
    {
        if (auto s_AIModifier = m_AIModifierBinding; s_AIModifier)
        {
            s_AIModifier.m_bIgnoreDistractions = false;
            s_AIModifier.m_bIgnoreAccidents = false;
            s_AIModifier.m_bIgnoreDeadBodies = false;
            s_AIModifier.m_bDisableDeadBodySensor = false;
            s_AIModifier.m_bIgnoreWeapons = false;
            s_AIModifier.m_bIgnoreTrespassing = false;
            s_AIModifier.m_bDeafAndBlind = false;
        }

        m_sMatch = BehaviorTreeMatch::BT_AMBIENCE;
        StopFollowHitman();
    }
};
