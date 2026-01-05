# Developing new Effects

to develop new effects, you'll need to write a new class implementing `IChaosEffect` and - specifically - the `Start` method.
from there, use the `REGISTER_CHAOS_EFFECT` macro to register your effect with the chaos mod.
reusable effect bases (like for interpolating in/out) are also available, and are located in the `Effects/Base` folder.
generally, if you're creating a new effect that has some reusable logic, consider adding it to the base effects for future use.

effects can also be parameterized (by adding constructor parameters) to allow for more flexible behavior.
in that case, use the `REGISTER_CHAOS_EFFECT_PARAM` macro instead.

refer to existing effects in the `Effects` folder for more complex examples.

## Effect Lifecycle

Effects are basically singleton instances, one per effect variant.
this approach was chosen over e.g. static classes to take advantage of class inheritance.

- Effects are instantiated and registered (by `REGISTER_CHAOS_EFFECT`) as soon as the mod dll is loaded. At this point, not much else is loaded, so Effects should __not__ do much work in their constructor.
- As ZHMModSDK initializes the mod, `OnModInitialized` is called. Here, effects may do any work that requires the SDK, but benefits from the engine not yet running (e.g. function hooking).
- After the Glacier2 engine is initialized, `OnEngineInitialized` is called. Any work requiring the engine (e.g. adding frame update listeners) may be done here.
- At some point between mod initialization and entering the scene, `LoadResources` is called. Effects may initializes any resource providers here.
- At about the first frame after scene load, `OnSceneEnter` is called. Effects may do any work requiring access to the full entity tree here.
- When the current scene unloads, `OnClearScene` is called. Effects may cleanup resources and entity references.
- On mod unload, `OnModUnload` is called. Any cleanup work (e.g. unhooking functions) may be done here.

## Effects vs Unlockers

effects are the main mechanism of the chaos mod.
they implement the actual chaotic behavior, and are selected at random during gameplay.

unlockers are a special, cut-down type of effect that aim to help the player in case of soft-lock situations.
they may be selected at any time by the player, even with other effects active.

## Custom Entities & Assets

effects that require custom entities or assets need to use the companion mod.
for usage of these assets, always use the `ZResourceProvider` / `ZTemplateEntitySpawner` classes from the companion mod, as they handle loading and unloading of resources properly.


## Minimal Example

```cpp
// MyChaosEffect.h
#pragma once
#include "IChaosEffect.h"

class MyChaosEffect : public IChaosEffect {
public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) override
    {
      return "My Effect";
    }
};

// MyChaosEffect.cpp
#include "MyChaosEffect.h"

void MyChaosEffect::Start() {
    // Your effect logic here
}

REGISTER_CHAOS_EFFECT(MyChaosEffect);
```
