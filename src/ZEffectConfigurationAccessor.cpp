#include "ZEffectConfigurationAccessor.h"

#include <IPluginInterface.h>

#include "Helpers/Utils.h"

ZEffectConfigurationAccessor::ZEffectConfigurationAccessor(IPluginInterface* p_pPlugin, const std::string p_sSection)
    : m_pPlugin(p_pPlugin),
      m_sSection(Utils::ToLower(p_sSection))
{
}

bool ZEffectConfigurationAccessor::HasSetting(const std::string& p_sName) const
{
    return SDK()->HasPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName));
}

void ZEffectConfigurationAccessor::SetBool(const std::string& p_sName, bool p_bValue)
{
    SDK()->SetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bValue);
}

bool ZEffectConfigurationAccessor::GetBool(const std::string& p_sName, bool p_bDefaultValue) const
{
    return SDK()->GetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bDefaultValue);
}
