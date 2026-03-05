#include "ZBlurryVisionEffect.h"

#include "Registry.h"

void ZBlurryVisionEffect::Start()
{
    auto& s_Params = GetParameters();
    s_Params.m_bGaussianBlurEnabled = true;
    s_Params.m_fGaussianBlurriness = 0.05f;

    BlendIn();
}

void ZBlurryVisionEffect::Stop()
{
    BlendOut();

    auto& s_Params = GetParameters();
    s_Params.m_bGaussianBlurEnabled = false;
}

REGISTER_CHAOS_EFFECT(ZBlurryVisionEffect)
