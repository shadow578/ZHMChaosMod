#pragma once
#include "Effects/Base/ZGravityModifierEffectBase.h"

class ZSimpleGravityModifierEffect : public ZGravityModifierEffectBase
{
public:
    ZSimpleGravityModifierEffect(const std::string p_sNameSuffix, const std::string p_sDisplayName, SVector3 p_vGravity)
        : ZGravityModifierEffectBase(),
        m_sNameSuffix(std::move(p_sNameSuffix)),
        m_sDisplayName(std::move(p_sDisplayName)),
        m_vGravity(p_vGravity)
    {
    }

    void Start() override;
    void Stop() override;

    std::string GetName() const override
    {
        return ZGravityModifierEffectBase::GetName() + "_" + m_sNameSuffix;
    }

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return m_sDisplayName;
    }

private:
    const std::string m_sNameSuffix;
    const std::string m_sDisplayName;
    const SVector3 m_vGravity;
};
