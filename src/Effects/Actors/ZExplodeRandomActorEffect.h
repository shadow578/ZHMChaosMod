#pragma once

#include "Effects/Base/ZExplosionEffectBase.h"

class ZExplodeRandomActorEffect : public ZExplosionEffectBase
{
public:
    void Start() override;
    void OnDrawDebugUI() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Random Actor Goes Boom";
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

private:
    TEntityRef<ZActor> m_rLastTarget = {};
    SMatrix m_LastTargetPos;
};