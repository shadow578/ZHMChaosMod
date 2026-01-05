#pragma once

namespace Utils
{
    /**
     * Cast data without checks or conversions.
     */
    template <typename T>
    static T* CastRaw(void* p_pData)
    {
        return reinterpret_cast<T*>(p_pData);
    }

    /**
     * Check if a pointer is a instance of a class.
     * @template Base Class to check if instance of.
     * @template T input type
     * @param p_pObj Object pointer to check.
     */
    template <typename Base, typename T>
    static bool IsInstanceOf(const T* p_pObj)
    {
        return dynamic_cast<const Base*>(p_pObj) != nullptr;
    }
};