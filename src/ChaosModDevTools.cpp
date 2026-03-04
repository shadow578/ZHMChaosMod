#include "ChaosMod.h"

#include <fstream>

#include <Logging.h>

#include "EffectRegistry.h"

#define TAG "[ChaosModDevTools] "

void ChaosMod::UpdateTestMode(const float32 p_fDeltaTime)
{
    if (!m_bTestmodeEnabled)
    {
        return;
    }

    m_fTestmodeTimeToNextEffect -= p_fDeltaTime;
    if (m_fTestmodeTimeToNextEffect <= 0.0f)
    {
        const auto& s_aEffects = EffectRegistry::GetInstance().GetEffects();
        if (s_aEffects.empty())
        {
            return;
        }

        const auto s_pEffect = s_aEffects[m_nTestmodeEffectIndex].get();
        if (s_pEffect)
        {
            Logger::Info(TAG "[TM] Activating effect '{}'", s_pEffect->GetName());
            m_pEffectForDebug = s_pEffect;
            ActivateEffect(s_pEffect, m_fTestmodeInterval - 1.0f);
        }

        m_nTestmodeEffectIndex = (m_nTestmodeEffectIndex + 1) % s_aEffects.size();
        m_fTestmodeTimeToNextEffect = m_fTestmodeInterval;
    }
}

void ChaosMod::GenerateAndOpenCompatibilityMatrix()
{
    // generate compatibility matrix as csv
    constexpr const char* c_sSeparator = ",";

    std::ofstream s_File("Effect_Compatibility_Matrix.csv", std::ios::trunc);

    s_File << "'Effect Compatibility'" << c_sSeparator;
    for (const auto& s_pEffectA : EffectRegistry::GetInstance().GetEffects())
    {
        s_File << "'" << s_pEffectA->GetName() << "'" << c_sSeparator;
    }
    s_File << std::endl;

    for (const auto& s_pEffectA : EffectRegistry::GetInstance().GetEffects())
    {
        s_File << "'" << s_pEffectA->GetName() << "'" << c_sSeparator;
        for (const auto& s_pEffectB : EffectRegistry::GetInstance().GetEffects())
        {
            const auto s_bCompatible = s_pEffectA->IsCompatibleWith(s_pEffectB.get());
            s_File << (s_bCompatible ? "X" : "-") << c_sSeparator;
        }
        s_File << std::endl;
    }

    s_File.close();

    // open with default program
    ShellExecuteA(nullptr, "open", "Effect_Compatibility_Matrix.csv", nullptr, nullptr, SW_SHOW);
}
