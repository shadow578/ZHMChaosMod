#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZOutfit.h>

#include <vector>
#include <string>

class ZAuthoritativeFigureEffect : public IChaosEffect
{
public:
    ZAuthoritativeFigureEffect();

    void Start() override;
    void Stop() override;
    bool Available() const override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Authoritative Figure";
    }

private:
    std::vector<std::string> m_aOutfitCommonNames;

    TEntityRef<ZGlobalOutfitKit> m_SelectedOutfit;
    bool m_bOriginalWeaponsAllowed;
    bool m_bOriginalAuthorityFigure;
};
