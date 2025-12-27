#pragma once

#include "Effects/Base/ZExplosionEffectBase.h"

class ZExplodeRandomActorEffect : public ZExplosionEffectBase
{
public:
    void Start() override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName() override
    {
        return "Random Actor Goes Boom";
    }

    EDuration GetDuration() override
    {
        return EDuration::OneShot;
    }

private:
    ZActor* m_pLastTarget = nullptr;
    SMatrix m_LastTargetPos;
};