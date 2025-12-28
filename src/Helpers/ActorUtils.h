#pragma once

#include "Globals.h"

#include "Glacier/ZActor.h"

namespace Utils
{
    std::vector<ZActor*> GetActors(const bool p_bIncludeDead, const bool p_bIncludePacified);

    ZActor* GetRandomActor(const bool p_bRequireAlive);
};
