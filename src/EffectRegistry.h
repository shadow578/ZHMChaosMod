#pragma once

#include <memory>
#include <vector>

#include "IChaosEffect.h"
#include "IUnlocker.h"
#include "IVotingIntegration.h"

#include <Logging.h>


class EffectRegistry
{
private:
    EffectRegistry() = default;
    std::vector<std::unique_ptr<IChaosEffect>> m_aEffects;
    std::vector<std::unique_ptr<IUnlocker>> m_aUnlockers;
	std::vector<std::unique_ptr<IVotingIntegration>> m_aVotingIntegrations;

public:
    static EffectRegistry& GetInstance()
    {
        // Note: g_Registry is *intentionally* leaked to avoid static deinitialization order issues.
        static EffectRegistry* g_Registry = new EffectRegistry();
        return *g_Registry;
    }

    void RegisterEffect(std::unique_ptr<IChaosEffect> p_Effect)
    {
        Logger::Debug("[EffectRegistry] Registered effect '{}'", p_Effect->GetName());
        m_aEffects.push_back(std::move(p_Effect));
    }

    void RegisterUnlocker(std::unique_ptr<IUnlocker> p_Unlocker)
    {
        Logger::Debug("[EffectRegistry] Registered unlocker '{}'", p_Unlocker->GetName());
        m_aUnlockers.push_back(std::move(p_Unlocker));
    }

    void RegisterVotingIntegration(std::unique_ptr<IVotingIntegration> p_Integration)
    {
        Logger::Debug("[EffectRegistry] Registered voting integration '{}'", p_Integration->GetName());
        m_aVotingIntegrations.push_back(std::move(p_Integration));
	}

    const std::vector<std::unique_ptr<IChaosEffect>>& GetEffects() const
    {
        return m_aEffects;
    }

    const std::vector<std::unique_ptr<IUnlocker>>& GetUnlockers() const
    {
        return m_aUnlockers;
    }

    const std::vector<std::unique_ptr<IVotingIntegration>>& GetVotingIntegrations() const
    {
        return m_aVotingIntegrations;
	}

    void Sort()
    {
        std::sort(
            m_aEffects.begin(),
            m_aEffects.end(),
            [](const std::unique_ptr<IChaosEffect>& a, const std::unique_ptr<IChaosEffect>& b)
            {
                return a->GetName() < b->GetName();
            }
		);

        std::sort(
            m_aUnlockers.begin(),
            m_aUnlockers.end(),
            [](const std::unique_ptr<IUnlocker>& a, const std::unique_ptr<IUnlocker>& b)
            {
                return a->GetName() < b->GetName();
            }
		);

        std::sort(
            m_aVotingIntegrations.begin(),
            m_aVotingIntegrations.end(),
            [](const std::unique_ptr<IVotingIntegration>& a, const std::unique_ptr<IVotingIntegration>& b)
            {
                return a->GetName() < b->GetName();
			}
		);
    }
};

struct EffectRegistrar
{
    explicit EffectRegistrar(std::unique_ptr<IChaosEffect> p_Effect)
    {
        EffectRegistry::GetInstance().RegisterEffect(std::move(p_Effect));
    }
};

struct UnlockerRegistrar
{
    explicit UnlockerRegistrar(std::unique_ptr<IUnlocker> p_Unlocker)
    {
        EffectRegistry::GetInstance().RegisterUnlocker(std::move(p_Unlocker));
    }
};

struct VotingIntegrationRegistrar
{
    explicit VotingIntegrationRegistrar(std::unique_ptr<IVotingIntegration> p_Integration)
    {
        EffectRegistry::GetInstance().RegisterVotingIntegration(std::move(p_Integration));
    }
};

/**
 * Register an effect class with the EffectRegistry.
 * Effecs should be registered in the global scope of the Effect's .cpp file.
 * Examples:
 * REGISTER_CHAOS_EFFECT(MySimpleEffect);
 * REGISTER_CHAOS_EFFECT_PARAM(MyParameterizedEffect, constructor_arg1, constructor_arg2);
 */
#define REGISTER_CHAOS_EFFECT(EFFECT_CLASS)                  \
    static EffectRegistrar g_EffectRegistrar_##EFFECT_CLASS( \
        std::make_unique<EFFECT_CLASS>()                     \
    );

#define REGISTER_CHAOS_EFFECT_PARAM(NAME, EFFECT_CLASS, ...)       \
    static EffectRegistrar g_EffectRegistrar_##EFFECT_CLASS##NAME( \
        std::make_unique<EFFECT_CLASS>(__VA_ARGS__)                \
    );

/**
 * Register an unlocker class with the EffectRegistry.
 * Unlockes are registered similarly to effects.
 */
#define REGISTER_CHAOS_UNLOCKER(UNLOCKER_CLASS)                    \
    static UnlockerRegistrar g_UnlockerRegistrar_##UNLOCKER_CLASS( \
        std::make_unique<UNLOCKER_CLASS>()                         \
    );

/**
 * Register a voting integration class with the Registry.
 * Voting integrations are registered similarly to effects, ... you get the idea.
 */
#define REGISTER_VOTING_INTEGRATION(INTEGRATION_CLASS)                                  \
    static VotingIntegrationRegistrar g_VotingIntegrationRegistrar_##INTEGRATION_CLASS( \
        std::make_unique<INTEGRATION_CLASS>()                                           \
    );
