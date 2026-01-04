#pragma once

#include <Globals.h>

#include <Glacier/ZActor.h>
#include <Glacier/ZMath.h>

namespace Utils
{
    std::vector<ZActor*> GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified);

    ZActor* GetRandomActor(const bool p_bRequireAlive);

    std::vector<std::pair<ZEntityRef, float32>> GetNearbyActors(
        const float4& p_vPosition,
        const int p_nMaxCount = INT_MAX,
        const float32 p_fRadius = FLT_MAX,
        const bool p_bIncludeDead = false, 
        const bool p_bIncludePacified = false);
};
