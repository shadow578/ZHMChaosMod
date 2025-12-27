#include "ZTimer.h"

#include "Globals.h"
#include "Glacier/ZGameLoopManager.h"

ZTimer::ZTimer(const ZTimerCallback p_Callback, const float32 p_fIntervalSeconds, const bool p_bStartEnabled)
    : m_bEnable(p_bStartEnabled),
    m_fIntervalSeconds(p_fIntervalSeconds),
    m_fElapsedSeconds(0),
    m_Callback(p_Callback)
{
}

void ZTimer::Initialize()
{
    const ZMemberDelegate<ZTimer, void(const SGameUpdateEvent&)> s_Delegate(this, &ZTimer::OnFrameUpdate);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);
}

void ZTimer::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    if (!m_bEnable)
    {
        return;
    }

    m_fElapsedSeconds += p_UpdateEvent.m_GameTimeDelta.ToSeconds();

    if (m_fElapsedSeconds >= m_fIntervalSeconds)
    {
        m_Callback();
        m_fElapsedSeconds = 0;
    }
}

float32 ZTimer::GetElapsedSeconds() const
{
    return m_fElapsedSeconds;
}
