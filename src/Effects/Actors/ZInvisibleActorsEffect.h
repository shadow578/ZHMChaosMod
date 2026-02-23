#pragma once
#include "Effects/UI/ZMinimapNoActorsEffect.h"

// Note: derive from ZMinimapNoActorsEffect so we can reuse minimap hiding logic.
// we don't want actors showing on the minimap if they're invisible.
class ZInvisibleActorsEffect : public ZMinimapNoActorsEffect
{
public:
    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Ghost Town";
    }

private:
    void SetAllActorsVisibility(const bool p_bVisible);
};
