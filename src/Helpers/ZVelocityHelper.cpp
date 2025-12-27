#include "ZVelocityTracker.h"

#include <Glacier/ZSpatialEntity.h>

void ZVelocityTracker::Update(const SGameUpdateEvent& p_UpdateEvent, const ZEntityRef p_Entity)
{
    if (!p_Entity)
    {
        return;
    }

    const auto s_SpatialEntity = p_Entity.QueryInterface<ZSpatialEntity>();
    if (!s_SpatialEntity)
    {
        return;
    }

    const auto s_CurrentPositionWM = s_SpatialEntity->GetWorldMatrix();
    Update(p_UpdateEvent, s_CurrentPositionWM.Trans);
}

void ZVelocityTracker::Update(const SGameUpdateEvent& p_UpdateEvent, const float4 p_Position)
{
    // first?
    if (!m_bLastPostionValid)
    {
        m_bLastPostionValid = true;
        m_LastPosition = p_Position;
        return;
    }

    const auto s_fDistance = float4::Distance(p_Position, m_LastPosition);
    m_LastPosition = p_Position;

    const STimeDistancePair s_NewPair{
        s_fDistance,
        p_UpdateEvent.m_GameTimeDelta.ToSeconds()
    };
    m_qSamples.push(s_NewPair);

    m_Sum.m_fDistance += s_NewPair.m_fDistance;
    m_Sum.m_fTimeDelta += s_NewPair.m_fTimeDelta;

    while (m_qSamples.size() > m_nSampleCount)
    {
        const auto s_RemovedPair = m_qSamples.front();
        m_Sum.m_fDistance -= s_RemovedPair.m_fDistance;
        m_Sum.m_fTimeDelta -= s_RemovedPair.m_fTimeDelta;

        m_qSamples.pop();
    }

    m_fVelocity = (m_Sum.m_fTimeDelta > 0.0f) ?
        (m_Sum.m_fDistance / m_Sum.m_fTimeDelta) :
        0.0f;
}

void ZVelocityTracker::Reset()
{
    while (!m_qSamples.empty())
    {
        m_qSamples.pop();
    }

    m_bLastPostionValid = false;
    m_Sum = { 0, 0 };
    m_fVelocity = 0.0f;
    m_LastPosition = float4();
}
