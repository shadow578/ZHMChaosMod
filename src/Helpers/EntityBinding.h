/**
 * Helper Macros to define bindings around ZEntityRef.
 * Example usage:
 * 
 * struct SMyEntityBinding
 * {
 *   BINDING_CONSTRUCTOR(SMyEntityBinding);
 * 
 *   PROPERTY(int32, m_nSomeIntProperty);
 *   PROPERTY(float32, m_fSomeFloatProperty);
 * 
 *   INPUT_PIN(SomeInputPin);
 *   OUTPUT_PIN(SomeOutputPin);
 * };
 * 
 * This allows accessing properties and signaling pins like:
 * SMyEntityBinding s_Binding(s_rEntity);
 * auto s_nValue = s_Binding.m_nSomeIntProperty;
 * s_Binding.SomeInputPin();
 * s_Binding.SomeOutputPin_Out();
 */
#pragma once
#include <Glacier/ZEntity.h>

#include "Helpers/EntityUtils.h"

#include <optional>

#define BINDING_CONSTRUCTOR(NAME)											\
	ZEntityRef m_rEntity;													\
	NAME##(ZEntityRef p_rEntity = {}) : m_rEntity(p_rEntity) {}             \
	operator bool() const {	return !!m_rEntity; }

#define PROPERTY(TYPE, NAME)    																		      \
     inline std::optional<TYPE> __##NAME##_Get() const { return Utils::GetProperty<TYPE>(m_rEntity, #NAME); } \
	 inline void __##NAME##_Set(const TYPE& value) { Utils::SetProperty<TYPE>(m_rEntity, #NAME, value); }     \
	 __declspec(property(get = __##NAME##_Get, put = __##NAME##_Set)) std::optional<TYPE> NAME;

#define PROPERTY_RO(TYPE, NAME)    																		      \
     inline std::optional<TYPE> __##NAME##_Get() const { return Utils::GetProperty<TYPE>(m_rEntity, #NAME); } \
	 __declspec(property(get = __##NAME##_Get)) std::optional<TYPE> NAME;

#define INPUT_PIN(NAME) \
	inline void NAME() { m_rEntity.SignalInputPin(#NAME); }

#define OUTPUT_PIN(NAME) \
	inline void NAME##_Out() { m_rEntity.SignalInputPin(#NAME); }