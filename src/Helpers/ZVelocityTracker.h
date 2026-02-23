#pragma once

#include <Glacier/ZEntity.h>
#include <Glacier/SGameUpdateEvent.h>
#include <Glacier/ZMath.h>

#include <queue>

/**
 * Helper for tracking the velocity of a spatial entity over time.
 */
class ZVelocityTracker
{
public:
    ZVelocityTracker(int p_nSampleCount = 10) :
        m_nSampleCount(p_nSampleCount)
    {
        Reset();
    }

    /**
     * Update velocity for a entity.
     * @param p_UpdateEvent Game update event.
     * @param p_Entity Entity reference.
     */
    void Update(const SGameUpdateEvent& p_UpdateEvent, const ZEntityRef p_Entity);

    /**
     * Update velocity for raw position.
     * @param p_UpdateEvent Game update event.
     * @param p_Position new position of the entity.
     */
    void Update(const SGameUpdateEvent& p_UpdateEvent, const float4 p_Position);

    /**
     * Reset velocity calculation.
     */
    void Reset();

    /**
     * Get the current (averaged) velocity of the entity.
     * Only if IsVelocityValid() == true.
     */
    float32 GetVelocity() const
    {
        return m_fVelocity;
    }

    /**
     * Is there enough data to calculate a sensible velocity?
     */
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
