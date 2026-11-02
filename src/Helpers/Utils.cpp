#include "Utils.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cctype>

void Utils::CopyToClipboard(const std::string& p_sData)
{
    if (!OpenClipboard(nullptr))
    {
        return;
    }

    EmptyClipboard();

    const auto s_hMem = GlobalAlloc(GMEM_MOVEABLE, p_sData.size() + 1);
    if (!s_hMem)
    {
        CloseClipboard();
        return;
    }

    const auto s_pData = GlobalLock(s_hMem);
    if (!s_pData)
    {
        CloseClipboard();
        GlobalFree(s_hMem);
        return;
    }

    memcpy(s_pData, p_sData.c_str(), p_sData.size() + 1);
    GlobalUnlock(s_hMem);

    SetClipboardData(CF_TEXT, s_hMem);
    CloseClipboard();
}

void Utils::OpenBrowser(const std::string& p_sUrl)
{
    ShellExecuteA(nullptr, "open", p_sUrl.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

uint64_t Utils::GetTimestamp()
{
    return GetTickCount64() / 1000; // ms to seconds
}

std::string Utils::ToLower(const std::string& p_sInput)
{
    std::string s_Result = p_sInput;
    std::transform(s_Result.begin(), s_Result.end(), s_Result.begin(), [](unsigned char c) { return std::tolower(c); });
    return s_Result;
}

bool Utils::IsReasonablePointer(const void* p_pPtr)
{
    // nullptr?
    if (!p_pPtr)
    {
        return false;
    }

    const auto s_nPtr = reinterpret_cast<uintptr_t>(p_pPtr);

    // common invalid pointers in Glacier2 / SDK
    // - 0xFFFF'FFFF'FFFF'FFFF (64-bit all bits set) - seen in BP factory pointers
    // - 0x0000'0000'FFFF'FFFF (64-bit lower 32 bits set) - seen for invalid entity refs
    if (s_nPtr == 0xFFFFFFFFFFFFFFFF || s_nPtr == 0x00000000FFFFFFFF)
    {
        return false;
    }

    // "The first 64KB of address space is never mapped in, so the lowest valid address you'd expect to see is 0x10000"
    // https://learn.microsoft.com/en-us/archive/msdn-magazine/2006/may/x64-starting-out-in-64-bit-windows-systems-with-visual-c
    if (s_nPtr < 0x10000)
    {
        return false;
    }

    // "A 64-bit process on 64-bit Windows has a virtual address space within the 128-terabyte range 0x000'00000000 through 0x7FFF'FFFFFFFF"
    // https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/virtual-address-spaces
    if (s_nPtr > 0x7FFFFFFFFFFF)
    {
        return false;
    }

    // aligned to 4 byte boundary?
    // not guranteed, but most pointers do follow this.
    if (s_nPtr % 4 != 0)
    {
        return false;
    }

    /*
    // check against application address space range.
    SYSTEM_INFO s_SysInfo;
    GetSystemInfo(&s_SysInfo);
    const auto s_nMinAppAddress = reinterpret_cast<uintptr_t>(s_SysInfo.lpMinimumApplicationAddress);
    const auto s_nMaxAppAddress = reinterpret_cast<uintptr_t>(s_SysInfo.lpMaximumApplicationAddress);
    if (s_nPtr < s_nMinAppAddress || s_nPtr > s_nMaxAppAddress)
    {
        return false;
    }
    */   

    // looks reasonable enough
    return true;
}
