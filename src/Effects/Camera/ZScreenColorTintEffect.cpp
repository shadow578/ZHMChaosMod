#include "ZScreenColorTintEffect.h"

#include "EffectRegistry.h"

void ZScreenColorTintEffect::Start()
{
	auto& s_Params = GetParameters();
	s_Params.Setm_bHDREnabled(true);
	s_Params.Setm_HDRColorTint(m_vColor);

	BlendIn();
}

void ZScreenColorTintEffect::Stop()
{
	BlendOut();

	auto& s_Params = GetParameters();
	s_Params.Setm_bHDREnabled(false);
}

void ZScreenColorTintEffect::OnDrawDebugUI()
{
	if (ImGui::ColorEdit3("Tint Color", &m_vColor.r))
	{
		Start();
	}

	ImGui::SeparatorText("ZRenderPostfilterEffectBase");
	ZRenderPostfilterEffectBase::OnDrawDebugUI();
}

REGISTER_CHAOS_EFFECT_PARAM(yellow, ZScreenColorTintEffect, "yellow", "Is this Mexico?", SColorRGB{ .r = 1.0f, .g = 1.0f, .b = 0.7f });
