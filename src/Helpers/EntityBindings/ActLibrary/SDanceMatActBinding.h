#pragma once
#include "Helpers/EntityBinding.h"

#include <Glacier/Enums.h>

class ZSpatialEntity;

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActLibrary/DanceMat of NPCActor, introduced by patch
// NPCActor_ActsLibrary.entity.patch.json in version 1.4.0 of Companion Mod.
struct SDanceMatActBinding
{
    BINDING_CONSTRUCTOR(SDanceMatActBinding);

    PROPERTY(ZActBehaviorEntity_EMovementType, m_MovementType); // either MT_WALK or MT_SNAP
    PROPERTY(bool, m_bActive);                                  // Whether the act is currently active. Prefer using Start / Cancel pins over setting this directly.
    PROPERTY(bool, m_bExpertMode);                              // Enable/disable expert dancer mode. Must be set before Start.
    PROPERTY(TEntityRef<ZSpatialEntity>, m_rWaypointSpatial);   // Location the act is performed at. Must be set before Start.

    INPUT_PIN(Start);  // Start the act.
    INPUT_PIN(Cancel); // Stop the act immediately.
};
