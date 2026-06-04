#pragma once
#include "IChaosEffect.h"

#include <vector>

#include "Entity/Bindings/SZRoomEntityBinding.h"

class ZInvisibleWorldGeometryEffect : public IChaosEffect
{
  public:
    void OnEnterScene() override;
    void OnClearScene() override;
    bool Available() const override;

    void Start() override;
    void Stop() override;

    std::string GetDisplayName(const bool p_bVoting) const override
    {
        return "Where did Everything go?";
    }

    EDuration GetDuration() const override
    {
        return EDuration::Short;
    }

  private:
    std::vector<SZRoomEntityBinding> m_aRooms;
    std::vector<SZRoomEntityBinding> m_aCurrentlyHiddenRooms;
};
