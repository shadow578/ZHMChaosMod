#include "ZQuickEntityLoader.h"

#include "Logging.h"

#define TAG "[ZQuickEntityLoader] "

ZQuickEntityLoader::ZQuickEntityLoader(std::string p_sName, std::string p_sQNJson)
    : m_sName(std::move(p_sName)),
    m_sQNJson(std::move(p_sQNJson))
{
}

ZQuickEntityLoader::~ZQuickEntityLoader()
{
    if (m_LoadThread.joinable())
    {
        m_LoadThread.join();
    }
}

void ZQuickEntityLoader::LoadAsync()
{   
    /*
    bool s_bExpected = false;
    if (!m_bIsStarted.compare_exchange_strong(s_bExpected, true))
    {
        return;
    }

    Logger::Debug(TAG "Starting load thead for '{}'", m_sName);

    m_LoadThread = std::thread(&ZQuickEntityLoader::Load, this);
    m_bIsReady.store(false, std::memory_order_release);
    m_bHasFailed.store(false, std::memory_order_release);
    */

    Load();    
}

void ZQuickEntityLoader::Load()
{
    if (Ready() || Failed())
    {
        // already loaded, abort
        return;
    }

    m_bIsStarted.store(true, std::memory_order_release);

    Logger::Debug(TAG "Loading Factory '{}' from QN", m_sName);
    if (!SDK()->LoadQnEntity(
        m_sQNJson,
        m_RTBpFactory,
        m_RTFactory
    ))
    {
        Logger::Debug(TAG "Loading Factory '{}' failed", m_sName);
        m_bHasFailed.store(true, std::memory_order_release);
        return;
    }

    m_bIsReady.store(true, std::memory_order_release);
}