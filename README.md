# Hitman 3 (WOA) ChaosMod

This is **ChaosMod**, a mod for Hitman 3!

the chaos mod mod injects random effects into your gameplay to create unpredictable, entertaining and often very difficult to handle scenarios.
heavily inspired by the [GTA V chaos mod](https://github.com/gta-chaos-mod/ChaosModV).

## Installation Instructions

1. Download the recommended\* version of [ZHMModSDK](https://github.com/OrfeasZ/ZHMModSDK) and install it.
2. Download the recommended\* version of `ChaosMod` and copy it to the ZHMModSDK `mods` folder (e.g. `C:\Program Files (x86)\Steam\steamapps\common\Hitman 3\Retail\mods`).
3. Download the recommended\* version of [Companion Mod](https://github.com/shadow578/ZHMChaosModCompanion) and install it using the [Simple Mod Framework](https://github.com/atampy25/simple-mod-framework) (optional, but recommended).
4. Run the game and once in the main menu, press the `~` key (`^` on QWERTZ layouts) and enable `ChaosMod` from the menu at the top of the screen (you may need to restart your game afterwards).
5. Check the `Enable` option in the ChaosMod menu to start the chaos!
6. Enjoy & good luck!

\* The version to use should generally be the latest release, but check ChaosMod release notes for any specific version requirements.

## Development

### Building

This mod uses the [ZHMModSDK](https://github.com/OrfeasZ/ZHMModSDK).
The development process is basically equal to any other ZHMModSDK mod.

#### 1. Clone this repository locally with all submodules.

You can either use `git clone --recurse-submodules` or run `git submodule update --init --recursive` after cloning.

#### 2. Install Visual Studio (any edition).

Make sure you install the C++ and game development workloads.

#### 3. Open the project in your IDE of choice.

See instructions for [Visual Studio](https://github.com/OrfeasZ/ZHMModSDK/wiki/Setting-up-Visual-Studio-for-development) or [CLion](https://github.com/OrfeasZ/ZHMModSDK/wiki/Setting-up-CLion-for-development).

#### 4. Setup Companion Mod

the chaos mod utilizes a companion mod for adding custom entities and assets to the game.
while many effects work without it, some require it to function properly.
for development, it is highly recommended to set up the companion mod as well.

to do so:

-   install SMF to your game directory into the `SMF` folder (e.g. `C:\Program Files (x86)\Steam\steamapps\common\Hitman 3\SMF`).
-   checkout the companion mod repository into the `mods` folder of SMF (e.g. `C:\Program Files (x86)\Steam\steamapps\common\Hitman 3\SMF\Mods\ZHMChaosModCompanion`).
-   any changes made to the companion mod will automatically be re-deployed when you build the chaos mod.

### Developing Effects

to develop new effects, you'll need to write a new class implementing `IChaosEffect` and - specifically - the `Start` method.
from there, use the `REGISTER_CHAOS_EFFECT` macro to register your effect with the chaos mod.
reusable effect bases (like for interpolating in/out) are also available, and are located in the `Effects/Base` folder.
generally, if you're creating a new effect that has some reusable logic, consider adding it to the base effects for future use.

minimal mod example:

```cpp
// MyChaosEffect.h
#pragma once
#include "IChaosEffect.h"

class MyChaosEffect : public IChaosEffect {
public:
    void Start() override;

    std::string GetDisplayName(const bool p_bVoting) override
    {
      return "My Chaos Effect";
    }
};

// MyChaosEffect.cpp
#include "MyChaosEffect.h"

void MyChaosEffect::Start() {
    // Your effect logic here
}

REGISTER_CHAOS_EFFECT(MyChaosEffect);
```

effects can also be parameterized (by adding constructor parameters) to allow for more flexible behavior.
in that case, use the `REGISTER_CHAOS_EFFECT_PARAM` macro instead.

refer to existing effects in the `Effects` folder for more complex examples.

#### Effect Lifecycle

1. Effects are instantiated and registered as soon as the mod dll is loaded. Effects shall never do any considerable work in the constructor. Effects always have only a single instance.
2. When ZHMModSDK initializes the mod, `OnModInitialized` is called. Here, effects can do any setup work that requires the SDK to be initialized (e.g. Hooking functions).
3. After the Glacier2 engine is initialized, `OnEngineInitialized` is called. Any work that requires the engine to be initialized (e.g. adding frame listeners) can be done here.
4. At some point between engine initialization and the first frame, `LoadResources` is called. Effects should check / load any resources they need here.
5. During play mode, `OnFrameUpdate` and `OnSlowUpdate` are called repeatedly (even on inactive effects!).
6. Upon selection of a effect, `Start` is called.
7. After a effect has timed out, `Stop` is called.
8. When the current scene is cleared, `OnClearScene` is called.
9. On mod unload, `OnModUnloading` is called. Any cleanup work can be done here (e.g. unhooking functions).

for any other functions, refer to the `IChaosEffect` interface definition.

#### Effects vs Unlockers

effects are the main mechanism of the chaos mod.
they implement the actual chaotic behavior, and are selected at random during gameplay.

unlockers are a special, cut-down type of effect that aim to help the player in case of soft-lock situations.
they may be selected at any time by the player, even with other effects active.

#### Custom Entities & Assets

effects that require custom entities or assets need to use the companion mod.
for usage of these assets, always use the `ZResourceProvider` / `ZTemplateEntitySpawner` classes from the companion mod, as they handle loading and unloading of resources properly.

## Related

-   [SMF Companion Mod](https://github.com/shadow578/ZHMChaosModCompanion)
-   [G2WwiseProject](https://github.com/shadow578/ChaosMod_G2WwiseProject)
