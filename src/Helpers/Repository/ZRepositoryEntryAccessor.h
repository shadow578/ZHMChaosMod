#pragma once
#include <Glacier/ZPrimitives.h>
#include <Glacier/ZObject.h>
#include <Glacier/TArray.h>

#include <optional>
#include <string>
#include <memory>

class ZRepositoryEntryAccessor
{
public:
	static std::unique_ptr<ZRepositoryEntryAccessor> FromEntry(const ZDynamicObject& p_Obj);
	ZRepositoryEntryAccessor(TArray<SDynamicObjectKeyValuePair>* p_pEntries) : m_pEntries(p_pEntries) {}

	SDynamicObjectKeyValuePair* GetPair(const std::string& p_sKey) const;

	bool HasKey(const std::string& p_sKey) const
	{
		return GetPair(p_sKey) != nullptr;
	}

	template <typename T>
	std::optional<T*> Get(const std::string& p_sKey) const
	{
		const auto& s_pPair = GetPair(p_sKey);
		if (!s_pPair)
		{
			return std::nullopt;
		}

		auto& s_Value = s_pPair->value;
		if (!s_Value.Is<T>()) {
			return std::nullopt;
		}

		return s_Value.As<T>();
	}

	template <typename T>
	bool Set(const std::string& p_sKey, const T& p_Value, const bool p_bIgnoreTypeMismatch = false)
	{
		const auto& s_pPair = GetPair(p_sKey);
		if (!s_pPair)
		{
			return false;
		}

		if (!p_bIgnoreTypeMismatch)
		{
			if (!s_pPair->value.Is<T>())
			{
				return false;
			}
		}

		s_pPair->value = p_Value;
		return true;
	}

private:
	TArray<SDynamicObjectKeyValuePair>* m_pEntries;
};
