#include "IChaosEffect.h"

#include <typeinfo>

#include "ZConfigurationAccessor.h"

std::string IChaosEffect::GetName() const
{
    std::string s_Name = typeid(*this).name();

    if (s_Name.find("class ") != std::string::npos)
    {
        s_Name.erase(0, 6 /* "class ".length() */);
    }

    return s_Name;
}

void IChaosEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfig)
{
    m_bIsEnabled = p_pConfig->GetBool("Enabled", true);
}

void IChaosEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfig)
{
    if (ImGui::Checkbox("Enabled", &m_bIsEnabled))
    {
        p_pConfig->SetBool("Enabled", m_bIsEnabled);
    }
}
