#include "ChaosMod.h"

#include "Logging.h"

#include <algorithm>
#include <random>

#include "EffectRegistry.h"
#include "Helpers/Math.h"

#define TAG "[ChaosMod] "

void ChaosMod::OnEffectTimerTrigger()
{
    auto* s_pVotingIntegration = GetCurrentVotingIntegration();
    if (!s_pVotingIntegration)
    {
        // something went very wrong...
		Logger::Error(TAG "ChaosMod::OnEffectTimerTrigger called but no voting integration is set!");
        return;
    }

    // select random from current vote
	// if no vote, skip and prepare next vote
    if (auto s_pSelectedEffect = s_pVotingIntegration->EndVote())
    {
        // fallback to randomly selected effect if selected is unavailable
        // no compatibility check here, as there should only be compatible effects in the vote
        // this should be fairly rare
        if (!s_pSelectedEffect->Available())
        {
            Logger::Warn(TAG "Selected effect '{}' is no longer available, selecting random fallback.", s_pSelectedEffect->GetName());
            const auto s_aFallbacks = GetRandomEffectSelection(1);
            if (!s_aFallbacks.empty())
            {
                s_pSelectedEffect = s_aFallbacks[0];
            }
        }

        ActivateEffect(s_pSelectedEffect);
    }

    // start next vote
    s_pVotingIntegration->StartVote(GetRandomEffectSelection(m_nVoteOptions));
}

void ChaosMod::ActivateEffect(IChaosEffect* p_pEffect)
{
    if (!p_pEffect || !p_pEffect->Available())
    {
        return;
    }

    Logger::Debug(TAG "Activating Effect '{}'.", p_pEffect->GetName());
    p_pEffect->Start();

    float32 s_fDuration;
    switch (p_pEffect->GetDuration())
    {
    case IChaosEffect::EDuration::OneShot:
    case IChaosEffect::EDuration::Short:
        s_fDuration = m_fFullEffectDuration * 0.5f;
        break;
    case IChaosEffect::EDuration::Full:
    default:
        s_fDuration = m_fFullEffectDuration;
        break;
    }

    const SActiveEffect s_ActiveEffect{
        .m_pEffect = p_pEffect,
        .m_fDuration = s_fDuration,
        .m_fTimeRemaining = s_fDuration
    };
    m_aActiveEffects.push_back(s_ActiveEffect);
}

void ChaosMod::UpdateEffectExpiration(const float32 p_fDeltaTime)
{
    for (auto& s_ActiveEffect : m_aActiveEffects)
    {
        if (!s_ActiveEffect.m_pEffect)
        {
            continue;
		}

        s_ActiveEffect.m_fTimeRemaining -= p_fDeltaTime;
        if (s_ActiveEffect.m_fTimeRemaining > 0.0f)
        {
            continue;
        }

        Logger::Debug(TAG "Stopping Effect '{}'.", s_ActiveEffect.m_pEffect->GetName());
        if (s_ActiveEffect.m_pEffect && s_ActiveEffect.m_pEffect->Available())
        {
            s_ActiveEffect.m_pEffect->Stop();
        }
    }

    // remove expired
    m_aActiveEffects.erase(
        std::remove_if(m_aActiveEffects.begin(), m_aActiveEffects.end(),
            [](const SActiveEffect& e) {
                return e.m_fTimeRemaining <= 0.0f;
            }),
        m_aActiveEffects.end()
    );
}

std::vector<IChaosEffect*> ChaosMod::GetRandomEffectSelection(const int p_nCount)
{
    const auto& s_aAllEffects = EffectRegistry::GetInstance().GetEffects();
    std::vector<IChaosEffect*> s_aSelection;

    if (s_aAllEffects.empty() || p_nCount <= 0)
    {
        return s_aSelection;
    }

    std::vector<IChaosEffect*> s_aShuffeled;
    s_aShuffeled.reserve(s_aAllEffects.size());

    for (const auto& s_pEffect : s_aAllEffects)
    {
        s_aShuffeled.push_back(s_pEffect.get());
    }

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::shuffle(s_aShuffeled.begin(), s_aShuffeled.end(), rng);

    for (auto* p_pEffect : s_aShuffeled)
    {
        // skip unavailable
        if (!p_pEffect->Available())
        {
            continue;
        }

        // skip incompatible
        if (!IsCompatibleWithAllActive(p_pEffect))
        {
            continue;
        }

        s_aSelection.push_back(p_pEffect);

        if (s_aSelection.size() >= p_nCount)
        {
            break;
        }
    }

    return s_aSelection;
}

bool ChaosMod::IsCompatibleWithAllActive(const IChaosEffect* p_pEffect)
{
    for (const auto& s_ActiveEffect : m_aActiveEffects)
    {
        if (!s_ActiveEffect.m_pEffect)
        {
            continue;
        }

        if (!p_pEffect->IsCompatibleWith(s_ActiveEffect.m_pEffect)
            || !s_ActiveEffect.m_pEffect->IsCompatibleWith(p_pEffect))
        {
            return false;
        }
    }

    return true;
}

float32 ChaosMod::GetEffectRemainingTime(const IChaosEffect* p_pEffect) const
{
    // debug takes precedence
    if (p_pEffect == m_pEffectForDebug)
    {
        return m_fDebugEffectRemainingTime;
    }

    for (const auto& s_ActiveEffect : m_aActiveEffects)
    {
        if (s_ActiveEffect.m_pEffect == p_pEffect)
        {
            return s_ActiveEffect.m_fTimeRemaining;
        }
    }

    // not active
    return 0.0f;
}
