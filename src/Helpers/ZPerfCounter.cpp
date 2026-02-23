#include "ZPerfCounter.h"

#include <Windows.h>

void ZPerfCounter::Start()
{
    LARGE_INTEGER s_nFrequency = {0};
    LARGE_INTEGER s_nStartTicks = {0};

	QueryPerformanceFrequency(&s_nFrequency);
	QueryPerformanceCounter(&s_nStartTicks);

    m_nFrequency = s_nFrequency.QuadPart;
    m_nStartTicks = s_nStartTicks.QuadPart;
}

double ZPerfCounter::Stop()
{
    LARGE_INTEGER s_nEndTicksLi = {0};
    QueryPerformanceCounter(&s_nEndTicksLi);
    const uint64_t s_nEndTicks = s_nEndTicksLi.QuadPart;

    const uint64_t s_nElapsedTicks = s_nEndTicks - m_nStartTicks;

    return static_cast<double>(s_nElapsedTicks) * 1'000'000.0 / static_cast<double>(m_nFrequency);
}
