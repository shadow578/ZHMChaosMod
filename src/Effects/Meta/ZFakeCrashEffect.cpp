#include "ZFakeCrashEffect.h"

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

#include "EffectRegistry.h"

constexpr DWORD c_THREAD_SUSPEND_MILLIS = 10000; // 10 seconds

void ZFakeCrashEffect::Start()
{
    SuspendAllThreads();
}

// based on https://github.com/gta-chaos-mod/ChaosModV/blob/master/ChaosMod/Effects/db/Misc/MiscFakeCrash.cpp
void ZFakeCrashEffect::SuspendAllThreads()
{
    std::vector<HANDLE> s_vThreadHandles;

    HANDLE s_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 s_ThreadEntry{};
    s_ThreadEntry.dwSize = sizeof(s_ThreadEntry);

    Thread32First(s_hSnapshot, &s_ThreadEntry);
    do
    {
        if (s_ThreadEntry.dwSize 
            >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(s_ThreadEntry.th32OwnerProcessID))
        {
            if (s_ThreadEntry.th32ThreadID != GetCurrentThreadId() &&
                s_ThreadEntry.th32OwnerProcessID == GetCurrentProcessId())
            {
                HANDLE s_hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, s_ThreadEntry.th32ThreadID);
                if (s_hThread)
                {
                    s_vThreadHandles.push_back(s_hThread);
                }
            }
        }

        s_ThreadEntry.dwSize = sizeof(s_ThreadEntry);
    }
    while (Thread32Next(s_hSnapshot, &s_ThreadEntry));


    for (auto s_hThread : s_vThreadHandles)
    {
        SuspendThread(s_hThread);
    }

    Sleep(c_THREAD_SUSPEND_MILLIS);

    for (auto s_hThread : s_vThreadHandles)
    {
        ResumeThread(s_hThread);
        CloseHandle(s_hThread);
    }

    CloseHandle(s_hSnapshot);
}

REGISTER_CHAOS_EFFECT(ZFakeCrashEffect)
