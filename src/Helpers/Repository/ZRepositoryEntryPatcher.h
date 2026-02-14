#pragma once
#include "ZRepositoryEntryAccessor.h"

#include <Logging.h>

#include <stack>

class ZRepositoryEntryPatcher 
{
public:
	ZRepositoryEntryPatcher(std::unique_ptr<ZRepositoryEntryAccessor> p_pEntry) : m_pEntry(std::move(p_pEntry)) {}
	~ZRepositoryEntryPatcher() 
	{
		if (m_bAutoRestore)
		{
			Restore();
		}
	}

	bool HasKey(const std::string& p_sKey) const
	{
		return m_pEntry->HasKey(p_sKey);
	}

	template <typename T>
	std::optional<T*> Get(const std::string& p_sKey) const
	{
		return m_pEntry->Get<T>(p_sKey);
	}

	template <typename T>
	bool Set(const std::string& p_sKey, const T& p_Value, const bool p_bIgnoreTypeMismatch = false)
	{
		// store original value for undo
		const auto& s_pPair = m_pEntry->GetPair(p_sKey);
		if (!s_pPair)
		{
			return false;
		}
		const auto s_Pair = *s_pPair; // copy to store original value

		// set new value
		if (!m_pEntry->Set<T>(p_sKey, p_Value, p_bIgnoreTypeMismatch))
		{
			return false;
		}

		// only store if set succeeded
		m_sUndoStack.push(s_Pair);
		return true;
	}

	void Restore()
	{
		while (!m_sUndoStack.empty())
		{
			const auto& s_Pair = m_sUndoStack.top();
			if (!m_pEntry->Set(s_Pair.sKey.c_str(), s_Pair.value, true))
			{
				Logger::Debug("[ZRepositoryEntryPatcher] Failed to restore original value for key = {}", s_Pair.sKey.c_str());
			}
			m_sUndoStack.pop();
		}
	}

	void SetAutoRestore(bool p_bAutoRestore)
	{
		m_bAutoRestore = p_bAutoRestore;
	}

private:
	std::unique_ptr<ZRepositoryEntryAccessor> m_pEntry;
	bool m_bAutoRestore = true;

	std::stack<SDynamicObjectKeyValuePair> m_sUndoStack;
};
