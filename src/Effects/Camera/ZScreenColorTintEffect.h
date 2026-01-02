#pragma once
#include "Base/ZRenderPostfilterEffectBase.h"

class ZScreenColorTintEffect : public ZRenderPostfilterEffectBase
{
public:
	ZScreenColorTintEffect(const std::string p_sNameSuffix, const std::string p_sDisplayName, const SColorRGB p_vColor)
		: ZRenderPostfilterEffectBase(),
		m_sNameSuffix(std::move(p_sNameSuffix)),
		m_sDisplayName(std::move(p_sDisplayName)),
		m_vColor(p_vColor)
	{
	}

	void Start() override;
	void Stop() override;
	void OnDrawDebugUI() override;

	std::string GetName() const override
	{
		return ZRenderPostfilterEffectBase::GetName() + "_" + m_sNameSuffix;
	}

	std::string GetDisplayName() const override
	{
		return m_sDisplayName;
	}

private:
	const std::string m_sNameSuffix;
	const std::string m_sDisplayName;
	SColorRGB m_vColor;
};
