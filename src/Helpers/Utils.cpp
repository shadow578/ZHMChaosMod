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
