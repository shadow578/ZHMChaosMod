#pragma once

namespace Utils
{
    template <typename T>
    static T* CastRaw(void* p_pData)
    {
        return reinterpret_cast<T*>(p_pData);
    }

    template <typename Base, typename T>
    static bool IsInstanceOf(const T* p_pOther)
    {
        return dynamic_cast<const Base*>(p_pOther) != nullptr;
    }
};