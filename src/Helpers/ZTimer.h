#pragma once

#include <functional>

#include <Glacier/SGameUpdateEvent.h>

typedef std::function<void()> ZTimerCallback;

/**
 * Calls a function every n Seconds, via FrameUpdate handler.
 * Can only be Initialize()-d after engine has initialized.
 */
class ZTimer
{
public:
    ZTimer(const ZTimerCallback p_Callback, const float32 p_fIntervalSeconds, const bool p_bStartEnabled = false);

    void Initialize();

    /**
     * Elapsed seconds. 
     * Reset after callback is invoked, thus this may be used to track delta time since last callback.
     */
    float32 GetElapsedSeconds() const;

    bool m_bEnable;
    float32 m_fIntervalSeconds;

private:
    void OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent);

    float32 m_fElapsedSeconds;
    ZTimerCallback m_Callback;
};