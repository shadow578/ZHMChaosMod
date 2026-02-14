#include "ZRepositoryEntryAccessor.h"

std::unique_ptr<ZRepositoryEntryAccessor> ZRepositoryEntryAccessor::FromEntry(const ZDynamicObject& p_Obj)
{
	auto* s_pEntries = p_Obj.As<TArray<SDynamicObjectKeyValuePair>>();
	if (!s_pEntries)
	{
		return nullptr;
	}

	return std::make_unique<ZRepositoryEntryAccessor>(s_pEntries);
}

SDynamicObjectKeyValuePair* ZRepositoryEntryAccessor::GetPair(const std::string& p_sKey) const
{
	for (auto& s_Pair : *m_pEntries)
	{
		const std::string s_sKey = std::string(s_Pair.sKey.c_str(), s_Pair.sKey.size());
		if (s_sKey == p_sKey)
		{
			return &s_Pair;
		}
	}

	return nullptr;
}
