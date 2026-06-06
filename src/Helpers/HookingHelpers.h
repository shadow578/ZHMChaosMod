/**
 * This header defines some helpers to work around limitations of the SDK's hooking system
 * mainly the part where, when registering a new hook, the context pointer is ignored during hook
 * de-duplication, which means you cannot normally have a base class define a hook.
 *
 * example:
 * header (ZSomeHookBaseEffect.h):
 * class ZSomeHookBaseEffect (...)
 * {
 *   // constructor and destructor must add/remove instances
 *   ZSomeHookBaseEffect(...)
 *   {
 *       ADD_BASE_CLASS_DETOUR_INSTANCE();
 *   }
 *   ~ZSomeHookBaseEffect()
 *   {
 *      REMOVE_BASE_CLASS_DETOUR_INSTANCE();
 *   }
 *
 * private:
 *   // declare list of instances, this must be done ONCE per base class
 *   DECLARE_BASE_CLASS_INSTANCES_HELPER(ZSomeHookBaseEffect);
 *
 *   // declare detours as you normally would, only instead of DECLARE_PLUGIN_DETOUR, use DECLARE_BASE_CLASS_DETOUR
 *   // you can have multiple detours per class
 *   DECLARE_BASE_CLASS_DETOUR(ZSomeHookBaseEffect, void, SomeHook, ...);
 * };
 *
 * cpp (ZSomeHookBaseEffect.cpp):
 * // define list of instances, this must be done ONCE per base class, in a cpp file
 * DEFINE_BASE_CLASS_INSTANCES_HELPER(ZSomeHookBaseEffect);
 *
 * // define your hook implementation as you normally would, but use DEFINE_BASE_CLASS_DETOUR instead of DEFINE_PLUGIN_DETOUR
 * // also, you need to specify the parameter names *without* type specifiers via MAKE_FORWARD_PARAMS.
 * DEFINE_BASE_CLASS_DETOUR(ZSomeHookBaseEffect, void, SomeHook, MAKE_FORWARD_PARAMS(...), ...)
 * {
 * ...
 * }
 */
#pragma once
#include <IPluginInterface.h> // for DECLARE_PLUGIN_DETOUR

#define DECLARE_BASE_CLASS_INSTANCES_HELPER(BaseClass) \
    static std::vector<BaseClass*> m_aInstancesForDetours;

#define DEFINE_BASE_CLASS_INSTANCES_HELPER(BaseClass) \
    std::vector<BaseClass*> BaseClass::m_aInstancesForDetours;

#define ADD_BASE_CLASS_DETOUR_INSTANCE() \
    m_aInstancesForDetours.push_back(this);

#define REMOVE_BASE_CLASS_DETOUR_INSTANCE() \
    m_aInstancesForDetours.erase(std::remove(m_aInstancesForDetours.begin(), m_aInstancesForDetours.end(), this), m_aInstancesForDetours.end());

#define DECLARE_BASE_CLASS_DETOUR(BaseClass, ReturnType, DetourName, ...)  \
    DECLARE_PLUGIN_DETOUR(BaseClass, ReturnType, DetourName, __VA_ARGS__); \
    HookResult<ReturnType> __fastcall DetourName##_Impl(                   \
        Hook<ReturnType(__VA_ARGS__)>* p_Hook,                             \
        __VA_ARGS__                                                        \
    );

#define DEFINE_BASE_CLASS_DETOUR(BaseClass, ReturnType, DetourName, ForwardParams, ...) \
    DEFINE_PLUGIN_DETOUR(BaseClass, ReturnType, DetourName, __VA_ARGS__)                \
    {                                                                                   \
        for (auto& s_pInstance : m_aInstancesForDetours)                                \
        {                                                                               \
            const auto s_Result = s_pInstance->DetourName##_Impl ForwardParams;         \
            if (s_Result.m_HasReturnVal)                                                \
            {                                                                           \
                return s_Result;                                                        \
            }                                                                           \
        }                                                                               \
                                                                                        \
        return {HookAction::Continue()};                                                \
    }                                                                                   \
    HookResult<ReturnType> __fastcall BaseClass::DetourName##_Impl(                     \
        Hook<ReturnType(__VA_ARGS__)>* p_Hook,                                          \
        __VA_ARGS__                                                                     \
    )

#define MAKE_FORWARD_PARAMS(...) \
    (p_Hook, __VA_ARGS__)
