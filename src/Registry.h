#pragma once
#include <memory>
#include <vector>
#include <string>

class IChaosEffect;
class IUnlocker;
class IVotingIntegration;

class Registry
{
  private:
    Registry() = default;
    std::vector<std::shared_ptr<IChaosEffect>> m_aEffects;
    std::vector<std::shared_ptr<IUnlocker>> m_aUnlockers;
    std::vector<std::shared_ptr<IVotingIntegration>> m_aVotingIntegrations;

  public:
    static Registry& GetInstance()
    {
        // Note: g_pRegistry is *intentionally* leaked to avoid static deinitialization order issues.
        static Registry* g_pRegistry = new Registry();
        return *g_pRegistry;
    }

    void RegisterEffect(std::shared_ptr<IChaosEffect> p_Effect);
    void RegisterUnlocker(std::shared_ptr<IUnlocker> p_Unlocker);
    void RegisterVotingIntegration(std::shared_ptr<IVotingIntegration> p_Integration);

    const std::vector<std::shared_ptr<IChaosEffect>>& GetEffects() const
    {
        return m_aEffects;
    }

    const std::vector<std::shared_ptr<IUnlocker>>& GetUnlockers() const
    {
        return m_aUnlockers;
    }

    const std::vector<std::shared_ptr<IVotingIntegration>>& GetVotingIntegrations() const
    {
        return m_aVotingIntegrations;
    }

    const std::shared_ptr<IChaosEffect>& GetEffectByName(const std::string& p_sName) const;
    const std::shared_ptr<IVotingIntegration>& GetVotingIntegrationByName(const std::string& p_sName) const;

    void Sort();
};

struct EffectRegistrar
{
    explicit EffectRegistrar(std::shared_ptr<IChaosEffect> p_Effect)
    {
        Registry::GetInstance().RegisterEffect(std::move(p_Effect));
    }
};

struct UnlockerRegistrar
{
    explicit UnlockerRegistrar(std::shared_ptr<IUnlocker> p_Unlocker)
    {
        Registry::GetInstance().RegisterUnlocker(std::move(p_Unlocker));
    }
};

struct VotingIntegrationRegistrar
{
    explicit VotingIntegrationRegistrar(std::shared_ptr<IVotingIntegration> p_Integration)
    {
        Registry::GetInstance().RegisterVotingIntegration(std::move(p_Integration));
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
        std::make_shared<EFFECT_CLASS>()                     \
    );

#define REGISTER_CHAOS_EFFECT_PARAM(NAME, EFFECT_CLASS, ...)       \
    static EffectRegistrar g_EffectRegistrar_##EFFECT_CLASS##NAME( \
        std::make_shared<EFFECT_CLASS>(__VA_ARGS__)                \
    );

/**
 * Register an unlocker class with the EffectRegistry.
 * Unlockes are registered similarly to effects.
 */
#define REGISTER_CHAOS_UNLOCKER(UNLOCKER_CLASS)                    \
    static UnlockerRegistrar g_UnlockerRegistrar_##UNLOCKER_CLASS( \
        std::make_shared<UNLOCKER_CLASS>()                         \
    );

/**
 * Register a voting integration class with the Registry.
 * Voting integrations are registered similarly to effects, ... you get the idea.
 */
#define REGISTER_VOTING_INTEGRATION(INTEGRATION_CLASS)                                  \
    static VotingIntegrationRegistrar g_VotingIntegrationRegistrar_##INTEGRATION_CLASS( \
        std::make_shared<INTEGRATION_CLASS>()                                           \
    );
