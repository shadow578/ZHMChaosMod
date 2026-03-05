#pragma once
#include <string>
#include <cstdint>

class IPluginInterface;

class ZConfigurationAccessor
{
  public:
    ZConfigurationAccessor(IPluginInterface* p_pPlugin, const std::string p_sSection);

    bool HasSetting(const std::string& p_sName) const;

    void SetString(const std::string& p_sName, const std::string& p_sValue);
    std::string GetString(const std::string& p_sName, const std::string& p_sDefaultValue) const;

    void SetInt(const std::string& p_sName, int64_t p_nValue);
    int64_t GetInt(const std::string& p_sName, int64_t p_nDefaultValue) const;

    void SetUInt(const std::string& p_sName, uint64_t p_nValue);
    uint64_t GetUInt(const std::string& p_sName, uint64_t p_nDefaultValue) const;

    void SetDouble(const std::string& p_sName, double p_fValue);
    double GetDouble(const std::string& p_sName, double p_fDefaultValue) const;

    void SetBool(const std::string& p_sName, bool p_bValue);
    bool GetBool(const std::string& p_sName, bool p_bDefaultValue) const;

  private:
    IPluginInterface* const m_pPlugin;
    std::string m_sSection;
};
