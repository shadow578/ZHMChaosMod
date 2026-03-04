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

void ZEffectConfigurationAccessor::SetString(const std::string& p_sName, const std::string& p_sValue)
{
    SDK()->SetPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_sValue);
}

std::string ZEffectConfigurationAccessor::GetString(const std::string& p_sName, const std::string& p_sDefaultValue) const
{
    return SDK()->GetPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_sDefaultValue).c_str();
}

void ZEffectConfigurationAccessor::SetInt(const std::string& p_sName, int64_t p_nValue)
{
    SDK()->SetPluginSettingInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nValue);
}

int64_t ZEffectConfigurationAccessor::GetInt(const std::string& p_sName, int64_t p_nDefaultValue) const
{
    return SDK()->GetPluginSettingInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nDefaultValue);
}

void ZEffectConfigurationAccessor::SetUInt(const std::string& p_sName, uint64_t p_nValue)
{
    SDK()->SetPluginSettingUInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nValue);
}

uint64_t ZEffectConfigurationAccessor::GetUInt(const std::string& p_sName, uint64_t p_nDefaultValue) const
{
    return SDK()->GetPluginSettingUInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nDefaultValue);
}

void ZEffectConfigurationAccessor::SetDouble(const std::string& p_sName, double p_fValue)
{
    SDK()->SetPluginSettingDouble(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_fValue);
}

double ZEffectConfigurationAccessor::GetDouble(const std::string& p_sName, double p_fDefaultValue) const
{
    return SDK()->GetPluginSettingDouble(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_fDefaultValue);
}

void ZEffectConfigurationAccessor::SetBool(const std::string& p_sName, bool p_bValue)
{
    SDK()->SetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bValue);
}

bool ZEffectConfigurationAccessor::GetBool(const std::string& p_sName, bool p_bDefaultValue) const
{
    return SDK()->GetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bDefaultValue);
}
