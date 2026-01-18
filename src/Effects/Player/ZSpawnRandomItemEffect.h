#pragma once
#include "Base/ZSpawnRepositoryItemEffectBase.h"

class ZSpawnRandomItemEffect : public ZSpawnRepositoryItemEffectBase
{
public:
    ZSpawnRandomItemEffect(const std::string p_sDisplayName, const int p_nCount)
        : ZSpawnRepositoryItemEffectBase(),
        m_sDisplayName(p_sDisplayName),
        m_pCount(p_nCount)
    {
    }

    void Start() override;

    std::string GetName() const override
    {
        return IChaosEffect::GetName() + "_" + std::to_string(m_pCount);
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_sDisplayName;
    }

    EDuration GetDuration() const override
    {
        return EDuration::OneShot;
    }

private:
    std::string m_sDisplayName;
    int m_pCount;
};
