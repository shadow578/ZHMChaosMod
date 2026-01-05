#pragma once

#include <Globals.h>

#include <Glacier/ZActor.h>
#include <Glacier/ZMath.h>

namespace Utils
{
    /**
     * Get all active actors in the scene.
     * @param p_bIncludeDead Include dead actors?
     * @param p_bIncludePacified Include pacified actors?
     */
    std::vector<ZActor*> GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified);

    /**
     * Get a random active actor in the current scene.
     * @param p_bRequireAlive Does the actor need to be not dead and not pacified?
     */
    ZActor* GetRandomActor(const bool p_bRequireAlive);

    /**
     * Get nearby actors given a center point. 
     * List is sorted by ascending distance to center point.
     * @param p_vPosition Center position.
     * @param p_nMaxCount Maximum number of actors.
     * @param p_fRadius Maximum distance a actor may be to be included.
     * @param p_bIncludeDead Include dead actors?
     * @param p_bIncludePacified Include pacified actors?
     */
    std::vector<std::pair<ZEntityRef, float32>> GetNearbyActors(
        const float4& p_vPosition,
        const int p_nMaxCount = INT_MAX,
        const float32 p_fRadius = FLT_MAX,
        const bool p_bIncludeDead = false, 
        const bool p_bIncludePacified = false);
};
