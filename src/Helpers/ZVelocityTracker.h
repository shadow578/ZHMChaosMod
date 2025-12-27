#pragma once

#include <Glacier/ZEntity.h>
#include <Glacier/SGameUpdateEvent.h>
#include <Glacier/ZMath.h>

#include <queue>

class ZVelocityTracker
{
public:
    ZVelocityTracker(int p_nSampleCount = 10) :
        m_nSampleCount(p_nSampleCount)
    {
        Reset();
    }

    void Update(const SGameUpdateEvent& p_UpdateEvent, const ZEntityRef p_Entity);
    void Update(const SGameUpdateEvent& p_UpdateEvent, const float4 p_Position);
    void Reset();

    float32 GetVelocity() const
    {
        return m_fVelocity;
    }

    bool IsVelocityValid() const
    {
        return m_bLastPostionValid && m_qSamples.size() == m_nSampleCount;
    }

private:
    struct STimeDistancePair
    {
        float32 m_fDistance;
        float32 m_fTimeDelta;
    };

    const int m_nSampleCount;
    std::queue<STimeDistancePair> m_qSamples;
    STimeDistancePair m_Sum{ 0, 0 };
    float32 m_fVelocity = 0.0f;

    bool m_bLastPostionValid = false;
    float4 m_LastPosition{};
};
