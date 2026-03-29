#pragma once

#include <Globals.h>

#include <Glacier/ZActor.h>
#include <Glacier/ZMath.h>

class ZSpatialEntity;

namespace Utils
{
    /**
     * Get all active actors in the scene.
     * @param p_bIncludeDead Include dead actors?
     * @param p_bIncludePacified Include pacified actors?
     */
    std::vector<TEntityRef<ZActor>> GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified);

    /**
     * Get a random active actor in the current scene.
     * @param p_bRequireAlive Does the actor need to be not dead and not pacified?
     */
    TEntityRef<ZActor> GetRandomActor(const bool p_bRequireAlive);

    /**
     * Get nearby actors given a center point.
     * List is sorted by ascending distance to center point.
     * @param p_vPosition Center position.
     * @param p_nMaxCount Maximum number of actors.
     * @param p_fRadius Maximum distance a actor may be to be included.
     * @param p_bIncludeDead Include dead actors?
     * @param p_bIncludePacified Include pacified actors?
     */
    std::vector<std::pair<TEntityRef<ZActor>, float32>> GetNearbyActors(
        const float4& p_vPosition,
        const int p_nMaxCount = INT_MAX,
        const float32 p_fRadius = FLT_MAX,
        const bool p_bIncludeDead = false,
        const bool p_bIncludePacified = false
    );

    /**
     * Get the head attachment point entity for a actor.
     * @param p_rActor Actor to get head attach for.
     * @return Entity ref of head attach, or empty if not found.
     */
    TEntityRef<ZSpatialEntity> GetActorHeadAttachEntity(const TEntityRef<ZActor> p_rActor);

    /**
     * Get the main weapon of an actor.
     * @param p_rActor Actor to get main weapon for.
     * @return Entity ref of main weapon, or empty if not found.
     */
    TEntityRef<ZHM5ItemWeapon> GetMainWeapon(const TEntityRef<ZActor> p_rActor);

    /**
     * Check if an actor is armed (has a main weapon equipped).
     * @param p_rActor Actor to check.
     */
    inline bool IsArmed(const TEntityRef<ZActor> p_rActor)
    {
        return !!GetMainWeapon(p_rActor);
    }

    /**
     * Add a weapon to the actor's inventory and equip it.
     * @param p_rActor Actor to add weapon to.
     * @param p_ridWeapon Repository ID of the weapon to add. Requires EEntryType::Weapon.
     * @param p_bReplaceMainWeapon Replace the currently equipped main weapon. if false, the weapon will only be equipped if the actor doesn't have a main weapon already.
     * @return Was the weapon successfully queued for equipping?
     * @note Depending on the actor, actually trying to use the weapon (e.g. via WeaponCustomControl) may crash the game.
     * @note When a weapon is already equipped and p_bReplaceMainWeapon is false, this function will return false.
     */
    bool AddAndEquipWeapon(TEntityRef<ZActor> p_rActor, const ZRepositoryID& p_ridWeapon, bool p_bReplaceMainWeapon = true);

    /**
     * Enable or disable ragdoll physics for an actor.
     * @param p_rActor Actor to enable/disable ragdoll physics for.
     * @param p_bEnablePhysicsSystem Whether to enable or disable the physics system.
     */
    bool SetRagdollPhysicsEnabled(TEntityRef<ZActor> p_rActor, const bool p_bEnablePhysicsSystem);
}; // namespace Utils
