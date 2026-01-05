# Setting up a developer environment

This mod uses the [ZHMModSDK](https://github.com/OrfeasZ/ZHMModSDK).
Thus, the development process in general is fairly similar to other ZHMModSDK mods.
There are however some additional steps needed due to the companion mod.

## 1. Clone this repository locally with all submodules.

You can either use `git clone --recurse-submodules` or run `git submodule update --init --recursive` after cloning.

## 2. Install Visual Studio (any edition).

Make sure you install the C++ and game development workloads.

## 3. Open the project in your IDE of choice.

See instructions for [Visual Studio](https://github.com/OrfeasZ/ZHMModSDK/wiki/Setting-up-Visual-Studio-for-development) or [CLion](https://github.com/OrfeasZ/ZHMModSDK/wiki/Setting-up-CLion-for-development).

## 4. Setup Companion Mod

the mod utilizes a [Simple Mod Framework](https://github.com/atampy25/simple-mod-framework) [companion mod](https://github.com/shadow578/ZHMChaosModCompanion) to add custom assets and entity templates.
while development is possible without it, it is __highly__ recommended to set up the companion mod as well:

1. install SMF into the `SMF` folder next to the game directory configured in the CMake project (e.g.: `C:\Program Files (x86)\Steam\steamapps\common\Hitman 3\SMF`).
2. clone the companion mod repository into the `mods` folder (e.g.: `C:\Program Files (x86)\Steam\steamapps\common\Hitman 3\SMF\Mods\`).
3. Re-configure the CMake project.
4. The companion mod will automatically deploy when you build the chaos mod.
