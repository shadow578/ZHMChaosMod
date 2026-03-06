#include "ZConfigurationAccessor.h"

#include <IPluginInterface.h> // for SDK()

#include "Helpers/Utils.h"

ZConfigurationAccessor::ZConfigurationAccessor(IPluginInterface* p_pPlugin, const std::string p_sSection)
    : m_pPlugin(p_pPlugin),
      m_sSection(Utils::ToLower(p_sSection))
{
}

bool ZConfigurationAccessor::HasSetting(const std::string& p_sName) const
{
    return SDK()->HasPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName));
}

void ZConfigurationAccessor::SetString(const std::string& p_sName, const std::string& p_sValue)
{
    SDK()->SetPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_sValue);
}

std::string ZConfigurationAccessor::GetString(const std::string& p_sName, const std::string& p_sDefaultValue) const
{
    return SDK()->GetPluginSetting(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_sDefaultValue).c_str();
}

void ZConfigurationAccessor::SetInt(const std::string& p_sName, int64_t p_nValue)
{
    SDK()->SetPluginSettingInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nValue);
}

int64_t ZConfigurationAccessor::GetInt(const std::string& p_sName, int64_t p_nDefaultValue) const
{
    return SDK()->GetPluginSettingInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nDefaultValue);
}

void ZConfigurationAccessor::SetUInt(const std::string& p_sName, uint64_t p_nValue)
{
    SDK()->SetPluginSettingUInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nValue);
}

uint64_t ZConfigurationAccessor::GetUInt(const std::string& p_sName, uint64_t p_nDefaultValue) const
{
    return SDK()->GetPluginSettingUInt(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_nDefaultValue);
}

void ZConfigurationAccessor::SetDouble(const std::string& p_sName, double p_fValue)
{
    SDK()->SetPluginSettingDouble(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_fValue);
}

double ZConfigurationAccessor::GetDouble(const std::string& p_sName, double p_fDefaultValue) const
{
    return SDK()->GetPluginSettingDouble(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_fDefaultValue);
}

void ZConfigurationAccessor::SetFloat(const std::string& p_sName, float p_fValue)
{
    SetDouble(p_sName, static_cast<float32>(p_fValue));
}

float ZConfigurationAccessor::GetFloat(const std::string& p_sName, float p_fDefaultValue) const
{
    return static_cast<float>(GetDouble(p_sName, p_fDefaultValue));
}

void ZConfigurationAccessor::SetBool(const std::string& p_sName, bool p_bValue)
{
    SDK()->SetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bValue);
}

bool ZConfigurationAccessor::GetBool(const std::string& p_sName, bool p_bDefaultValue) const
{
    return SDK()->GetPluginSettingBool(m_pPlugin, m_sSection, Utils::ToLower(p_sName), p_bDefaultValue);
}
