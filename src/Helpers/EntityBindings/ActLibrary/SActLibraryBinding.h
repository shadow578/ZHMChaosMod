#pragma once
#include "Helpers/EntityBinding.h"
#include "SDanceMatActBinding.h"

// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
// Sub-entity _ChaosMod_ActLibrary of NPCActor, introduced by patch
// NPCActor_ActsLibrary.entity.patch.json in version 1.4.0 of Companion Mod.
struct SActLibraryBinding
{
    BINDING_CONSTRUCTOR(SActLibraryBinding);

    SUB_ENTITY_BINDING(SDanceMatActBinding, m_DanceMatAct, 0xcafeea36ac1680ff) // _ChaosMod_ActLibrary/DanceMat
};
