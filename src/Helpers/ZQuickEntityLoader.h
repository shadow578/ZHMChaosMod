#pragma once

#include <string>
#include <atomic>
#include <thread>

#include "IPluginInterface.h"

#include <Glacier/ZResource.h>
#include <Glacier/EntityFactory.h>

class ZQuickEntityLoader
{
public:
    ZQuickEntityLoader(std::string p_sName, std::string p_sQNJson);
    ~ZQuickEntityLoader();

    ZQuickEntityLoader(const ZQuickEntityLoader&) = delete;
    ZQuickEntityLoader& operator=(const ZQuickEntityLoader&) = delete;

    /**
     * Start loading QN entity synchronously.
     * The function will return once loading finished. Ready() will be true after this.
     * Must NOT be called if LoadAsync() was called before.
     */
    void Load();

    /**
     * Start loading QN entity asynchronously.
     * The function will return immediately. Ready() can be polled to check when loading finished.
     */
    void LoadAsync();

    /**
     * Has loading the QN entity started?
     * True immediately after Load() or LoadAsync() was called.
     */
    bool Loading() const
    {
        return m_bIsStarted.load(std::memory_order_acquire);
    }

    /**
     * Is the QN entity ready to be retrived via Get functions?
     * When HasFailed(), Ready() will always be false.
     */
    bool Ready() const
    {
        return m_bIsReady.load(std::memory_order_acquire);
    }

    /**
     * Did loading the QN entity fail?
     */
    bool Failed() const
    {
        return m_bHasFailed.load(std::memory_order_acquire);
    }

    /**
     * Get the loaded entity factory. Only when Ready() is true.
     */
    TResourcePtr<ZTemplateEntityFactory> GetFactory()
    {
        return m_RTFactory;
    }

    /**
     * Get the loaded entity blueprint factory. Only when Ready() is true.
     */
    TResourcePtr<ZTemplateEntityBlueprintFactory> GetBlueprintFactory()
    {
        return m_RTBpFactory;
    }

private:
    std::string m_sName;
    std::string m_sQNJson;
    TResourcePtr<ZTemplateEntityFactory> m_RTFactory;
    TResourcePtr<ZTemplateEntityBlueprintFactory> m_RTBpFactory;

    std::thread m_LoadThread;
    std::atomic<bool> m_bIsStarted{ false };
    std::atomic<bool> m_bIsReady{ false };
    std::atomic<bool> m_bHasFailed{ false };
};