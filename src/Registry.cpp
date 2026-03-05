#include "Registry.h"

#include <Logging.h>

#include "IChaosEffect.h"
#include "IUnlocker.h"
#include "IVotingIntegration.h"

#define TAG "[Registry] "

void Registry::RegisterEffect(std::shared_ptr<IChaosEffect> p_Effect)
{
    for (const auto& s_pOtherEffect : m_aEffects)
    {
        if (s_pOtherEffect->GetName() == p_Effect->GetName())
        {
            Logger::Error(
                TAG "Effect with name '{}' is already registered, skipping duplicate",
                p_Effect->GetName()
            );
            return;
        }
    }

    Logger::Debug(TAG "Registered effect '{}'", p_Effect->GetName());
    m_aEffects.push_back(std::move(p_Effect));
}

void Registry::RegisterUnlocker(std::shared_ptr<IUnlocker> p_Unlocker)
{
    for (const auto& s_pOtherUnlocker : m_aUnlockers)
    {
        if (s_pOtherUnlocker->GetName() == p_Unlocker->GetName())
        {
            Logger::Error(
                TAG "Unlocker with name '{}' is already registered, skipping duplicate",
                p_Unlocker->GetName()
            );
            return;
        }
    }

    Logger::Debug(TAG "Registered unlocker '{}'", p_Unlocker->GetName());
    m_aUnlockers.push_back(std::move(p_Unlocker));
}

void Registry::RegisterVotingIntegration(std::shared_ptr<IVotingIntegration> p_Integration)
{
    for (const auto& s_pOtherIntegration : m_aVotingIntegrations)
    {
        if (s_pOtherIntegration->GetName() == p_Integration->GetName())
        {
            Logger::Error(
                TAG "Voting integration with name '{}' is already registered, skipping duplicate",
                p_Integration->GetName()
            );
            return;
        }
    }

    Logger::Debug(TAG "Registered voting integration '{}'", p_Integration->GetName());
    m_aVotingIntegrations.push_back(std::move(p_Integration));
}

const std::shared_ptr<IChaosEffect>& Registry::GetEffectByName(const std::string& p_sName) const
{
    for (const auto& s_pEffect : m_aEffects)
    {
        if (s_pEffect->GetName() == p_sName)
        {
            return s_pEffect;
        }
    }

    return nullptr; // FIXME: returning address of local variable or temporary
}

const std::shared_ptr<IVotingIntegration>& Registry::GetVotingIntegrationByName(const std::string& p_sName) const
{
    for (const auto& s_pIntegration : m_aVotingIntegrations)
    {
        if (s_pIntegration->GetName() == p_sName)
        {
            return s_pIntegration;
        }
    }

    return nullptr; // FIXME: returning address of local variable or temporary
}

void Registry::Sort()
{
    std::sort(
        m_aEffects.begin(),
        m_aEffects.end(),
        [](const std::shared_ptr<IChaosEffect>& a, const std::shared_ptr<IChaosEffect>& b) {
            return a->GetName() < b->GetName();
        }
    );

    std::sort(
        m_aUnlockers.begin(),
        m_aUnlockers.end(),
        [](const std::shared_ptr<IUnlocker>& a, const std::shared_ptr<IUnlocker>& b) {
            return a->GetName() < b->GetName();
        }
    );

    std::sort(
        m_aVotingIntegrations.begin(),
        m_aVotingIntegrations.end(),
        [](const std::shared_ptr<IVotingIntegration>& a, const std::shared_ptr<IVotingIntegration>& b) {
            return a->GetName() < b->GetName();
        }
    );
}
