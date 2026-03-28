#pragma once
#include <cstdint>

/// Entity IDs used in the game and companion.
/// IDs are namespaced by Source::EntityTemplate::Name, where:
/// - Source is the source that introduces the entity
/// - EntityTemplate is the name of the .entitytemplate that defines the entity (full path in docstring)
/// - Name is the literal name of the entity ID, with entity path described in the docstring
namespace EntityId
{
    typedef uint64_t entityId_t;

    /// Entities defined in base game
    namespace HM3
    {
        /// [assembly:/_pro/scenes/bricks/gameessentialsbase.brick].pc_entitytype
        namespace GameEssentials
        {
            /// Scene > [GameEssentials.brick] > UI > MASTER_ROOT
            constexpr entityId_t MasterRoot = 0x77bed13890286553;

            /// Scene > [GameEssentials.brick] > UI > IngameMenu > MapMenu > InGameMapSetup > Minimap
            constexpr entityId_t Minimap = 0xccda0a613b5d88cb;
        } // namespace GameEssentials

        /// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
        namespace NPCActor
        {
            /// NPCActor > PhysicsSystem
            constexpr entityId_t PhysicsSystem = 0x66aaaad61b6a51a5;

            /// NPCActor > Head
            constexpr entityId_t Head = 0x5f46597848b36b38;
        } // namespace NPCActor

        /// [assembly:/_pro/characters/templates/hero/agent47/agent47.template?/agent47_default.entitytemplate].pc_entitytype
        namespace Agent47Default
        {
            /// Agent47_Default > Head
            constexpr entityId_t Head = 0x0ff5798a35665af2;
        } // namespace Agent47Default

        /// [assembly:/templates/core/hm5camera.template?/compositeentity_norenderdestination.entitytemplate].pc_entitytype
        namespace CompositeEntityNoRenderDestination
        {
            /// CompositeEntity_NoRenderDestination > HM5MainCamera01
            constexpr entityId_t HM5MainCamera01 = 0x151b9072f9d0dc77;
        } // namespace CompositeEntityNoRenderDestination

        /// [assembly:/_pro/effects/templates/logic/fx_logic_physicsworld.template?/fx_logic_physicsworld.entitytemplate].pc_entitytype
        namespace FXLogicPhysicsWorld
        {
            /// FX_Logic_PhysicsWorld > PhysicsWorld
            constexpr entityId_t PhysicsWorld = 0x859611037148f21b;
        } // namespace FXLogicPhysicsWorld

        /// [assembly:/_pro/effects/templates/postfilters/fx_pf_postfiltercontroller.template?/fx_pf_postfiltercontroller.entitytemplate].pc_entitytype
        namespace FXPFPostFilterController
        {
            /// FX_PF_PostfilterController > CustomPFController > PostfilterLayerEntity11
            constexpr entityId_t CustomPFLayer = 0x87805acc41dd775e;
        } // namespace FXPFPostFilterController

        /// [assembly:/_pro/design/templates/camera/pictureinpicture.template?/pip_camera_custom_event.entitytemplate].pc_entitytype
        namespace PiPCameraCustomEvent
        {
            /// PIP_Camera_Custom_Event > Lookattrigger
            constexpr entityId_t LookAtTrigger = 0x2c2b10f742467d79;
        } // namespace PiPCameraCustomEvent

        /// [assembly:/_pro/effects/templates/fire/fx_fireworks.template?/fx_fireworks_launchpad.entitytemplate].pc_entitytype
        namespace FXFireworksLaunchpad
        {
            /// FX_Fireworks_LaunchPad > Fireworks
            constexpr entityId_t Fireworks = 0x3fa6ea87b6e7bfec;

            /// FX_Fireworks_LaunchPad > Fireworks > firework_swirlmachine_a
            constexpr entityId_t FireworkSwirlMachine1 = 0x0d26e9029d9cdb1a;

            /// FX_Fireworks_LaunchPad > Fireworks > firework_swirlmachine_a01
            constexpr entityId_t FireworkSwirlMachine2 = 0x03c72250d57f0356;

            /// FX_Fireworks_LaunchPad > Fireworks > TimerSimple
            constexpr entityId_t TimerSimple = 0xbc53160f645abffc;

            /// FX_Fireworks_LaunchPad > Fireworks > TimerSimple01
            constexpr entityId_t TimerSimple01 = 0x36a50d14be2f2f88;

            /// FX_Fireworks_LaunchPad > Fireworks > TimerSimple02
            constexpr entityId_t TimerSimple02 = 0xdd9b974ddf3feda5;

            /// FX_Fireworks_LaunchPad > Fireworks > TimerSimple03
            constexpr entityId_t TimerSimple03 = 0x3ac42cc89a95ba81;
        } // namespace FXFireworksLaunchpad
    } // namespace HM3

    /// Entities defined in ZHMChaosMod SMF Companion Mod
    namespace CompanionMod
    {
        /// [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
        namespace NPCActor
        {
            /// NPCActor_FollowPlayerHelper.entity.patch
            /// NPCActor > _ChaosMod_FollowPlayerHelper
            constexpr entityId_t FollowPlayerHelper = 0xcafee66c8c3ff219;

            /// NPCActor_ShootAtHelper.entity.patch
            /// NPCActor > _ChaosMod_ShootAtHelper
            constexpr entityId_t ShootAtHelper = 0xcafe18d8b8ef4c24;
        } // namespace NPCActor
    } // namespace CompanionMod
} // namespace EntityId
