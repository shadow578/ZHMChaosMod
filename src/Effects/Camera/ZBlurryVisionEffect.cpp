#include "ZBlurryVisionEffect.h"

#include "EffectRegistry.h"

void ZBlurryVisionEffect::Start()
{
	auto& s_Params = GetParameters();
	s_Params.Setm_bGaussianBlurEnabled(true);
	s_Params.Setm_fGaussianBlurriness(0.05f);

	BlendIn();
}

void ZBlurryVisionEffect::Stop()
{
	BlendOut();

	auto& s_Params = GetParameters();
	s_Params.Setm_bGaussianBlurEnabled(false);
}

REGISTER_CHAOS_EFFECT(ZBlurryVisionEffect)
