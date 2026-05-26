#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/ZSpatialEntity.h>
#include <Glacier/Enums.h>

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActLibrary/LambicDance of NPCActor, introduced by patch
// NPCActor_ActLibrary.entity.patch.json in version 1.4.0 of Companion Mod.
struct SLambicDanceActEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZActBehaviorEntity_EMovementType, m_MovementType);  // NPC movement type. either MT_WALK or MT_SNAP. Cannot be changed after Start is triggered.
    PROPERTY_RO(bool, m_bActive);                                // Whether act is active.
    PROPERTY_RO(TEntityRef<ZSpatialEntity>, m_rWaypointSpatial); // Spatial that NPC will perform act at. Location cannot be changed after Start is triggered.

    INPUT_PIN(Start);  // Start act. NPC will stop current act and start moving towards m_rWaypointSpatial, then perform dance animation there.
    INPUT_PIN(Cancel); // Cancel act. NPC will resume normal behaviour.

    EXPOSED_INTERFACE(ZSpatialEntity, Spatial);
};
