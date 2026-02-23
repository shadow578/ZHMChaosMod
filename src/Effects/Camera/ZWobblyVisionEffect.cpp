#include "ZWobblyVisionEffect.h"

#include "EffectRegistry.h"

void ZWobblyVisionEffect::Start()
{
	auto& s_Params = GetParameters();
	s_Params.m_bDistortionBarrelEnabled = true; // hide glitches at screen edges a bit
	s_Params.m_fDistortionBarrelScale = 0.5f;
	s_Params.m_vDistortionBarrel = SVector2(0.2f, 0.1f);
	s_Params.m_bGaussianBlurEnabled = true; // blur to hide artifacts a bit
	s_Params.m_fGaussianBlurriness = 0.025f;

	s_Params.m_bDistortionWobbleEnabled = true; // go wobbly
	s_Params.m_bDistortionWobbleUseRealTime = true;
	s_Params.m_fDistortionWobbleScale = 1.3f;
	s_Params.m_vDistortionWobbleWaveLength = SVector2(0.1f, 0.1f);
	s_Params.m_vDistortionWobbleSpeed = SVector2(1.5f, 2.1f);

	BlendIn();
}

void ZWobblyVisionEffect::Stop()
{
	BlendOut();

	auto& s_Params = GetParameters();
	s_Params.m_bDistortionBarrelEnabled = false;
	s_Params.m_bGaussianBlurEnabled = false;
	s_Params.m_bDistortionWobbleEnabled = false;
}

REGISTER_CHAOS_EFFECT(ZWobblyVisionEffect)
