#include "ZTimer.h"

#include "Globals.h"
#include "Glacier/ZGameLoopManager.h"

void ZTimer::Initialize()
{
    const ZMemberDelegate<ZTimer, void(const SGameUpdateEvent&)> s_Delegate(this, &ZTimer::OnFrameUpdate);
    Globals::GameLoopManager->RegisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);
}

void ZTimer::Deinitialize()
{
    const ZMemberDelegate<ZTimer, void(const SGameUpdateEvent&)> s_Delegate(this, &ZTimer::OnFrameUpdate);
    Globals::GameLoopManager->UnregisterFrameUpdate(s_Delegate, 1, EUpdateMode::eUpdatePlayMode);
}

void ZTimer::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    if (!m_bEnable)
    {
        return;
    }

    if (m_eTimeMode == ETimeMode::RealTime)
    {
        m_fElapsedSeconds += p_UpdateEvent.m_RealTimeDelta.ToSeconds();
    }
    else
    {
        m_fElapsedSeconds += p_UpdateEvent.m_GameTimeDelta.ToSeconds();
	}

    if (m_fElapsedSeconds >= m_fIntervalSeconds)
    {
        m_Callback();
        Reset();
    }
}

