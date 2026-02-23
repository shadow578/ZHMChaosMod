#pragma once

#include <functional>

#include <Glacier/SGameUpdateEvent.h>

typedef std::function<void()> ZTimerCallback;

/**
 * Helper for calling a function on a regular interval, via FrameUpdate handler.
 * Initialize() *must* be called manually in or after OnEngineInitialized! 
 * Deinitialize() *must* be called manually in mod unload!
 */
class ZTimer
{
public:
    enum class ETimeMode
    {
        GameTime,
        RealTime
	};

    ZTimer(const ZTimerCallback p_Callback, const float32 p_fIntervalSeconds, const ETimeMode p_eTimeMode = ETimeMode::GameTime, const bool p_bStartEnabled = false)
        : m_bEnable(p_bStartEnabled),
        m_fIntervalSeconds(p_fIntervalSeconds),
        m_fElapsedSeconds(0),
		m_eTimeMode(p_eTimeMode),
        m_Callback(p_Callback)
    {
    }

    void Initialize();
    void Deinitialize();

    void Reset()
    {
		m_fElapsedSeconds = 0;
    }

    /**
     * Get elapsed seconds. 
     * NOTE: Reset after callback is invoked, thus this may be used to track delta time since last callback.
     */
    float32 GetElapsedSeconds() const
    {
		return m_fElapsedSeconds;
    }

    bool m_bEnable;
    float32 m_fIntervalSeconds;
	ETimeMode m_eTimeMode;

private:
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent);

    float32 m_fElapsedSeconds;
    ZTimerCallback m_Callback;
};