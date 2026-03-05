#include "ChaosMod.h"

#include <fstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <shellapi.h>

#include <Logging.h>

#include "Registry.h"

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
        const auto& s_aEffects = Registry::GetInstance().GetEffects();
        if (s_aEffects.empty())
        {
            return;
        }

        const auto s_pEffect = s_aEffects[m_nTestmodeEffectIndex];
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
    constexpr const char* c_sFilename = "Effect_Compatibility_Matrix.csv";

    std::ofstream s_File(c_sFilename, std::ios::trunc);
    if (!s_File.is_open())
    {
        Logger::Error(TAG "Failed to open '{}' for writing compatibility", c_sFilename);
        return;
    }

    s_File << "'Effect Compatibility'" << c_sSeparator;
    for (const auto& s_pEffectA : Registry::GetInstance().GetEffects())
    {
        s_File << "'" << s_pEffectA->GetName() << "'" << c_sSeparator;
    }
    s_File << std::endl;

    for (const auto& s_pEffectA : Registry::GetInstance().GetEffects())
    {
        s_File << "'" << s_pEffectA->GetName() << "'" << c_sSeparator;
        for (const auto& s_pEffectB : Registry::GetInstance().GetEffects())
        {
            const auto s_bCompatible = s_pEffectA->IsCompatibleWith(s_pEffectB.get());
            s_File << (s_bCompatible ? "X" : "-") << c_sSeparator;
        }
        s_File << std::endl;
    }

    s_File.close();

    // open with default program
    ShellExecuteA(nullptr, "open", c_sFilename, nullptr, nullptr, SW_SHOW);
}
