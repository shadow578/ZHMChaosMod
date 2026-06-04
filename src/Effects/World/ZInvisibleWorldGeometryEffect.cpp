#include "ZInvisibleWorldGeometryEffect.h"

#include <Logging.h>
#include <imgui.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/Math.h"

#define TAG "[ZInvisibleWorldGeometryEffect] "

void ZInvisibleWorldGeometryEffect::OnEnterScene()
{
    m_aRooms.clear();
    m_aCurrentlyHiddenRooms.clear();

    const auto s_aRooms = Utils::ZEntityFinder()
                              .EntityType("ZRoomEntity")
                              .Find();

    for (auto& s_rRoom : s_aRooms)
    {
        m_aRooms.emplace_back(SZRoomEntityBinding(s_rRoom));
    }

    Logger::Debug(TAG "Found {} room entities!", m_aRooms.size());
}

void ZInvisibleWorldGeometryEffect::OnClearScene()
{
    m_aRooms.clear();
    m_aCurrentlyHiddenRooms.clear();
}

bool ZInvisibleWorldGeometryEffect::Available() const
{
    return IChaosEffect::Available() && !m_aRooms.empty();
}

void ZInvisibleWorldGeometryEffect::Start()
{
    if (!m_aCurrentlyHiddenRooms.empty())
    {
        Stop();
    }

    for (auto& s_Room : m_aRooms)
    {
        // only apply to currently visible rooms, as Stop() would otherwise make by-default invisible rooms visible.
        if (auto s_boVisible = s_Room.m_bVisible; s_boVisible.has_value())
        {
            if (!s_boVisible.value())
            {
                continue;
            }
        }

        s_Room.MakeInvisible();
        m_aCurrentlyHiddenRooms.emplace_back(s_Room);
    }
}

void ZInvisibleWorldGeometryEffect::Stop()
{
    for (auto& s_Room : m_aCurrentlyHiddenRooms)
    {
        s_Room.MakeVisible();
    }
    m_aCurrentlyHiddenRooms.clear();
}

REGISTER_CHAOS_EFFECT(ZInvisibleWorldGeometryEffect)
