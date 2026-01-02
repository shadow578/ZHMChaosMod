#include "ZWobblyVisionEffect.h"

#include "EffectRegistry.h"

void ZWobblyVisionEffect::Start()
{
	auto& s_Params = GetParameters();
	s_Params.Setm_bDistortionBarrelEnabled(true); // hide glitches at screen edges a bit
	s_Params.Setm_fDistortionBarrelScale(0.5f);
	s_Params.Setm_vDistortionBarrel(SVector2(0.2f, 0.1f));
	s_Params.Setm_bGaussianBlurEnabled(true); // blur to hide artifacts a bit
	s_Params.Setm_fGaussianBlurriness(0.025f);

	s_Params.Setm_bDistortionWobbleEnabled(true); // go wobbly
	s_Params.Setm_bDistortionWobbleUseRealTime(true);
	s_Params.Setm_fDistortionWobbleScale(1.3f);
	s_Params.Setm_vDistortionWobbleWaveLength(SVector2(0.1f, 0.1f));
	s_Params.Setm_vDistortionWobbleSpeed(SVector2(1.5f, 2.1f));

	BlendIn();
}

void ZWobblyVisionEffect::Stop()
{
	BlendOut();

	auto& s_Params = GetParameters();
	s_Params.Setm_bDistortionBarrelEnabled(false);
	s_Params.Setm_bGaussianBlurEnabled(false);
	s_Params.Setm_bDistortionWobbleEnabled(false);
}

REGISTER_CHAOS_EFFECT(ZWobblyVisionEffect)
