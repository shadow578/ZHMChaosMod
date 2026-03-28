#pragma once
#include "Entity/Bindings/EntityBinding.h"

class ZSpatialEntity;

// [assembly:/_pro/chaosmod/sfxplayer.entitytemplate].pc_entitytype
// introduced by companion mod, SFXPlayer.entity.json
struct SSoundFXPlayerEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(ZRuntimeResourceID, m_pMainEvent); // Sound to play. Requires .pc_wwisebank resource that is already loaded.

    INPUT_PIN(StartSFX);   // Start Playing SFX (via ZAudioEventEntity)
    INPUT_PIN(StartMusic); // Start Music (via ZMusicEventEntity)
    INPUT_PIN(Stop);       // Stop SFX and Music

    EXPOSED_INTERFACE(ZSpatialEntity, Spatial);
};
