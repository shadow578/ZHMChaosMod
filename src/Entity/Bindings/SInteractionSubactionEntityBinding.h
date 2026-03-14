#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [assembly:/_pro/design/gamecore/interactionbase.template?/interaction_subaction_noinvestigate.entitytemplate].pc_entityblueprint (partial)
struct SInteractionSubactionEntityBinding : SEntityBinding
{
    PROPERTY(TEntityRef<ZSpatialEntity>, m_rContextObjectSpatial); // Spatial where the prompt appears
    PROPERTY(ZString, m_sPromptText);                              // main prompt text
    PROPERTY(ZString, m_sPromptDescriptionText);                   // prompt subtext

    INPUT_PIN(Execute); // Start the interaction
};
