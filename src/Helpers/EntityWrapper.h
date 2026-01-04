/**
 * Helper Macros to define wrappers around ZEntityRef.
 * Example usage:
 * 
 * struct SMyEntityWrapper
 * {
 *   WRAPPER_CONSTRUCTOR(SMyEntityWrapper);
 * 
 *   PROPERTY(int32, m_nSomeIntProperty);
 *   PROPERTY(float32, m_fSomeFloatProperty);
 * 
 *   INPUT_PIN(SomeInputPin);
 *   OUTPUT_PIN(SomeOutputPin);
 * };
 */
#pragma once
#include <Glacier/ZEntity.h>

#include "Helpers/EntityUtils.h"

#include <optional>

#define WRAPPER_CONSTRUCTOR(NAME)											\
	ZEntityRef m_rEntity;													\
	NAME##(ZEntityRef p_rEntity = {}) : m_rEntity(p_rEntity) {} \
	operator bool() const {	return !!m_rEntity; }

#define PROPERTY(TYPE, NAME)    																		 \
     inline std::optional<TYPE> Get##NAME() const { return Utils::GetProperty<TYPE>(m_rEntity, #NAME); } \
	 inline void Set##NAME(const TYPE& value) { Utils::SetProperty<TYPE>(m_rEntity, #NAME, value); }

#define INPUT_PIN(NAME) \
	inline void SignalInputPin_##NAME() { m_rEntity.SignalInputPin(#NAME); }

#define OUTPUT_PIN(NAME) \
	inline void SignalOutputPin_##NAME() { m_rEntity.SignalInputPin(#NAME); }
