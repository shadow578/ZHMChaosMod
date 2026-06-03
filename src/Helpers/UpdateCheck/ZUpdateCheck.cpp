#include "ZUpdateCheck.h"

#include <Logging.h>

#include <nlohmann/json.hpp>
#include <ixwebsocket/IXHttpClient.h>

#include <BuildInfo.h>

#define TAG "[ZUpdateCheck] "

using json = nlohmann::json;

void ZUpdateCheck::CheckUpdatesAsync()
{
    std::lock_guard s_Lock(m_ResultMutex);
    if (m_eResult != EResult::None)
    {
        Logger::Debug(TAG "Update check already performed, skipping start of another check.");
        return;
    }

    if (m_UpdateCheckThread.joinable())
    {
        Logger::Error(TAG "Update check thread already running, but state does not match!");
        return;
    }

    m_UpdateCheckThread = std::thread(&ZUpdateCheck::CheckUpdatesInternal, this);
    m_eResult = EResult::Checking;
}

void ZUpdateCheck::CheckUpdatesInternal()
{
    Logger::Info(TAG "Starting update check for {}/{}...", BuildInfo::c_sRepoOwner, BuildInfo::c_sRepoName);

    std::string s_sUrl = "https://api.github.com/repos/"
                         + BuildInfo::c_sRepoOwner
                         + "/"
                         + BuildInfo::c_sRepoName
                         + "/releases/latest";
    Logger::Debug(TAG "Update check URL: {}", s_sUrl);

    ix::HttpClient s_Client;
    ix::HttpRequestArgsPtr s_pRequest = s_Client.createRequest();
    const auto s_pResponse = s_Client.get(s_sUrl, s_pRequest);
    if (!s_pResponse)
    {
        Logger::Error(TAG "Failed to perform update check HTTP request!");

        {
            std::lock_guard s_Lock(m_ResultMutex);
            m_eResult = EResult::Failed;
        }
        return;
    }

    if (s_pResponse->statusCode < 200 || s_pResponse->statusCode >= 300)
    {
        Logger::Error(TAG "update check failed: {} {}", s_pResponse->statusCode, s_pResponse->body);

        {
            std::lock_guard s_Lock(m_ResultMutex);
            m_eResult = EResult::Failed;
        }
        return;
    }

    const auto s_ResponseJson = json::parse(s_pResponse->body);

    const auto s_sName = s_ResponseJson.value("name", "");
    const auto s_sTagName = s_ResponseJson.value("tag_name", "");
    const auto s_bIsPrerelease = s_ResponseJson.value("prerelease", false);
    const auto s_bIsDraft = s_ResponseJson.value("draft", false);

    if (s_sName.empty() || s_sTagName.empty())
    {
        Logger::Error(TAG "Invalid response from update check: missing name or tag_name field!");
        {
            std::lock_guard s_Lock(m_ResultMutex);
            m_eResult = EResult::Failed;
        }
        return;
    }

    Logger::Debug(TAG "Got latest release: name={}, tag_name={} (build={}), prerelease={}, draft={}", s_sName, s_sTagName, BuildInfo::c_sBuildTag, s_bIsPrerelease, s_bIsDraft);

    auto s_bUpdateAvailable = false;
    if (!s_bIsPrerelease && !s_bIsDraft)
    {
        s_bUpdateAvailable = s_sTagName != BuildInfo::c_sBuildTag;
    }

    {
        std::lock_guard s_Lock(m_ResultMutex);
        m_sLatestTag = s_sTagName;
        m_sLatestVersionName = s_sName;
        m_eResult = s_bUpdateAvailable ? EResult::UpdateAvailable : EResult::UpToDate;
    }
}

std::string ZUpdateCheck::GetUpdateUrl() const
{
    return "https://github.com/"
           + BuildInfo::c_sRepoOwner
           + "/"
           + BuildInfo::c_sRepoName
           + "/releases/latest";
}
