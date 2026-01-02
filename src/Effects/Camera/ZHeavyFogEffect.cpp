#include "ZHeavyFogEffect.h"

#include "EffectRegistry.h"

void ZHeavyFogEffect::Start()
{
	auto& s_Params = GetParameters();
	s_Params.Setm_bFogEnabled(true); // enable global fog
	s_Params.Setm_bFogGlobal(true);
	s_Params.Setm_fFogDensity(0.15f); // thick, but not solid fog
	s_Params.Setm_vFogNearDistances(SVector2(2.0f, 10.0f));
	s_Params.Setm_fFogTurbulenceOpacity(0.7f);
	s_Params.Setm_FogColor({ .r = 0.95f, .g = 1.0f, .b = 1.0f }); // gray, lightly blueish
	s_Params.Setm_vFogTurbulenceNearScale(SVector3(10.0f, 10.0f, 10.0f)); // misc. defaults
	s_Params.Setm_vFogTurbulenceVelocity(SVector3(0.3f, 0.5f, 0.1f));
	s_Params.Setm_fFogTurbulenceChaos(5.0f);

	BlendIn(5.0f);
}

void ZHeavyFogEffect::Stop()
{
	BlendOut();

	auto& s_Params = GetParameters();
	s_Params.Setm_bFogEnabled(false);
}

REGISTER_CHAOS_EFFECT(ZHeavyFogEffect)
