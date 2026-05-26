#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/ZSpatialEntity.h>

// [assembly:/_pro/chaosmod/seagull_mayhem.entitytemplate].pc_entitytype
// introduced by companion mod in Version 1.4.0.
struct SSeagullMayhemEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    PROPERTY(TEntityRef<ZSpatialEntity>, m_eidParent); // Parent spatial of the effect. Seagulls will spawn around this spatial.
    PROPERTY(float32, m_fTimeToLiveSeconds);           // Time until seagull particles are despawned. Since there's no way to stop the effect, you'll have to set this to match the effect duration. Default is 30 seconds.

    INPUT_PIN(Activate); // Start spawning the particles. Once activated, particles will be emitted for 5 seconds. After m_fTimeToLiveSeconds, the particles will automatically despawned. After initial particle emmission, Activate() can be called again.

    EXPOSED_INTERFACE(ZSpatialEntity, Spatial);
};
