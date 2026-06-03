#pragma once
#include <thread>
#include <mutex>

class ZUpdateCheck
{
  public:
    enum class EResult
    {
        None,
        Checking,
        UpToDate,
        UpdateAvailable,
        Failed
    };

    /**
     * Start an asynchronous check for updates.
     * @note this function will only perform the check once, even when called multiple times.
     */
    void CheckUpdatesAsync();

    /**
     * Get the result of the update check.
     */
    EResult GetResult() const
    {
        std::lock_guard s_Lock(m_ResultMutex);
        return m_eResult;
    }

    /**
     * Get the display name of the latest version as displayed on Github releases.
     */
    std::string GetLatestVersion() const
    {
        std::lock_guard s_Lock(m_ResultMutex);
        return m_sLatestVersionName;
    }

    /**
     * Get the update URL that a user can visit to download the latest version. 
     */
    std::string GetUpdateUrl() const;

  private:
    std::thread m_UpdateCheckThread;

    mutable std::recursive_mutex m_ResultMutex;
    EResult m_eResult = EResult::None;
    std::string m_sLatestTag;
    std::string m_sLatestVersionName;

    void CheckUpdatesInternal();
};
