#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include "SValueBoolBasicEntityBinding.h"

// [assembly:/_pro/design/gamecore/interactionbase.template?/interaction_subaction_noinvestigate.entitytemplate].pc_entityblueprint (partial)
struct SInteractionSubactionEntityBinding : SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(TEntityRef<ZSpatialEntity>, m_rContextObjectSpatial); // Spatial where the prompt appears
    PROPERTY(ZString, m_sPromptText);                              // main prompt text
    PROPERTY(ZString, m_sPromptDescriptionText);                   // prompt subtext

    INPUT_PIN(Execute); // Trigger the interaction

    // NOTE: conditions are valuebool_operation, so their value cannot be changed by pins.
    SUBENTITY_BINDING(SValueBoolBasicEntityBinding, VisibleCondition, 0x2170f51ac21b7278);           // interaction is visible
    SUBENTITY_BINDING(SValueBoolBasicEntityBinding, UseableCondition, 0x412423ecae3ee883);           // interaction can be used normally (e.g. required item is present)
    SUBENTITY_BINDING(SValueBoolBasicEntityBinding, VisibleAndUseableCondition, 0x2e7713d6e7765766); // both visible and useable conditions are satisfied
};
