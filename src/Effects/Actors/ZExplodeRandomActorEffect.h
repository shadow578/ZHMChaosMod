#pragma once

#include "Effects/Base/ZExplosionEffectBase.h"

class ZExplodeRandomActorEffect : public ZExplosionEffectBase
{
public:
    void Start() override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName() const override
    {
        return "Random Actor Goes Boom";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

private:
    ZActor* m_pLastTarget = nullptr;
    SMatrix m_LastTargetPos;
};