#pragma once
#include "Entity/Bindings/EntityBinding.h"

class ZSpatialEntity;

// [modules:/zragdollattacherentity.class].pc_entitytype
// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActorUtils/RagdollAttacher of NPCActor, introduced by patch
// NPCActor/ActorUtils.entity.patch.json in version 1.6.0 of Companion Mod.
struct SZRagdollAttacherEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZString, m_BoneNameA);       // Bone name. only "head" is known to work.
    PROPERTY(ZString, m_BoneNameB);       // Bone name. only "head" is known to work.
    PROPERTY(ZString, m_ragdollBoneName); // Bone name. only "head" is known to work.
    PROPERTY(ZEntityRef, m_linkedEntity); // Linked entity the ragdoll is attached to. Set to a custom spatial entity in ActorUtils patch.
    PROPERTY(ZEntityRef, m_ragdoll);      // Ragdoll entity to attach. Set to parent actor in ActorUtils patch.

    INPUT_PIN(Attach); // Attach the ragdoll.
    INPUT_PIN(Detach); // Detach the ragdoll.

    EXPOSED_INTERFACE(ZSpatialEntity, Spatial); // Spatial that moves m_linkedEntity.
};
