#pragma once
#include <string>

class IPluginInterface;

class ZEffectConfigurationAccessor
{
  public:
    ZEffectConfigurationAccessor(IPluginInterface* p_pPlugin, const std::string p_sSection);

    bool HasSetting(const std::string& p_sName) const;

    void SetBool(const std::string& p_sName, bool p_bValue);
    bool GetBool(const std::string& p_sName, bool p_bDefaultValue) const;

  private:
    IPluginInterface* const m_pPlugin;
    std::string m_sSection;
};
