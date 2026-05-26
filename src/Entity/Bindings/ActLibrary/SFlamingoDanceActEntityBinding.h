#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/Enums.h>

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActLibrary/FlamingoDance of NPCActor, introduced by patch
// NPCActor_ActLibrary.entity.patch.json in version 1.4.0 of Companion Mod.
struct SFlamingoDanceActEntityBinding : public SEntityBinding
{
    constexpr static int32 MODE_DEFAULT = 0;    // Act_MR_Stand_Mascot_Entertain - Start
    constexpr static int32 MODE_HANDS_AIR = 1;  // Act_MR_Stand_Mascot_Entertain - Start_HandsAir
    constexpr static int32 MODE_SPIN = 2;       // Act_MR_Stand_Mascot_Entertain - Start_Spin
    constexpr static int32 MODE_FLAP = 3;       // Act_MR_Stand_Mascot_Entertain - Start_Flap
    constexpr static int32 MODE_RACE = 4;       // Act_MR_Stand_Mascot_Entertain - Start_Race
    constexpr static int32 MODE_EXERCISE = 5;   // Act_MR_Stand_Mascot_Entertain - Start_Exercise
    constexpr static int32 MODE_SILLY_WALK = 6; // Act_MR_Stand_Mascot_Entertain - Start_SillyWalk

    using SEntityBinding::SEntityBinding;

    PROPERTY(ZActBehaviorEntity_EMovementType, m_MovementType);  // NPC movement type. either MT_WALK or MT_SNAP. Cannot be changed after Start is triggered.
    PROPERTY_RO(bool, m_bActive);                                // Whether act is active.
    PROPERTY_RO(TEntityRef<ZSpatialEntity>, m_rWaypointSpatial); // Spatial that NPC will perform act at. Location cannot be changed after Start is triggered.

    PROPERTY(int32, m_nMode); // Flamingo dance mode / type. Not well tested yet, keep at MODE_DEFAULT to be safe. Refer to MODE_* constants for available values. Cannot be changed after Start is triggered.

    INPUT_PIN(Start);  // Start act. NPC will stop current act and start moving towards m_rWaypointSpatial, then perform dance animation there.
    INPUT_PIN(Cancel); // Cancel act. NPC will resume normal behaviour.

    EXPOSED_INTERFACE(ZSpatialEntity, Spatial);
};
