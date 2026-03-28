#pragma once
#include "Entity/Bindings/EntityBinding.h"

// [modules:/zaicrippleentity.class].pc_entitytype
struct SAICrippleEntityBinding : SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    INPUT_PIN(SetHeroInvincible); // make player invincible
    INPUT_PIN(SetHeroVulnerable); // make player vulnerable again after being invincible
    INPUT_PIN(SetHeroHidden);     // make player invisible to AI
    INPUT_PIN(SetHeroVisible);    // make player visible to AI again after being invisible
    INPUT_PIN(DisableBehaviors);  // disable NPC AI behaviors
    INPUT_PIN(EnableBehaviors);   // re-enable NPC AI behaviors after being disabled
};
