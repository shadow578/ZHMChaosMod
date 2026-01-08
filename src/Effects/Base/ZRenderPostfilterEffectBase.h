#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/SColorRGB.h>
#include "Helpers/EntityUtils.h"
#include "Helpers/EntityBinding.h"

/**
 * Reusable base providing access to the render postfilter graph.
 * Allows setting custom parameters via GetParameters(), and enabling and disabling
 * using BlendIn(), BlendOut() and BlendInOut() methods.
 * Lifecycle is handled automatically.
 */
class ZRenderPostfilterEffectBase : public virtual IChaosEffect
{
public:
	ZRenderPostfilterEffectBase() {}

	void OnEnterScene() override;
	void OnClearScene() override;
	bool Available() const override;

	void OnDrawDebugUI() override;

protected: // Wrappers

#pragma region Layer Entity Wrapper
	struct SLayerEntityBinding
	{
		BINDING_CONSTRUCTOR(SLayerEntityBinding);

		PROPERTY(float32, m_fBlendInDuration);
		PROPERTY(float32, m_fBlendOutDuration);
		PROPERTY(float32, m_fBlendSustainDuration);

		INPUT_PIN(BlendIn);
		INPUT_PIN(BlendOut);
		INPUT_PIN(BlendInOut);
	};
#pragma endregion

#pragma region Parameters Entity Wrapper
	struct SParametersEntityBinding
	{
		// note: this is only partial, only things i care about and got to work are here
		BINDING_CONSTRUCTOR(SParametersEntityBinding);

		PROPERTY(bool, m_bFogEnabled); // enable fog
		PROPERTY(bool, m_bFogGlobal); // fog is global / only in some area box ?
		PROPERTY(float32, m_fFogDensity); // fog density. 0.0 - 1.0 for normal use, at 3.0 screen is solid fog
		PROPERTY(SVector2, m_vFogNearDistances); // fog start distance; x/y not quite sure, maybe turbulent/dense fog?
		PROPERTY(SColorRGB, m_FogColor);
		PROPERTY(float32, m_fFogTurbulenceOpacity); // opacity of turbulent fog layer (0.0 - 1.0)
		PROPERTY(SVector3, m_vFogTurbulenceNearScale); // scale of turbulent fog "clouds"
		PROPERTY(SVector3, m_vFogTurbulenceVelocity); // movement speed & direction of turbulent fog "clouds"
		PROPERTY(float32, m_fFogTurbulenceChaos); // randomness in turbulent fog movement (?)


		PROPERTY(bool, m_bNoiseEnabled); // enable noise/ static overlay
		PROPERTY(float32, m_fNoiseIntensity); // intensity of noise. 0.0 - 1.0, but can go higher
		PROPERTY(float32, m_fNoiseScale); // size of noise "blocks". 0.0 - 1.0, but can go higher


		PROPERTY(bool, m_bVignetteEnabled); // enable vignette effect
		PROPERTY(float32, m_fVignetteStrenght); // how far the vignette reaches into the screen. 0.0 - 1.0
		PROPERTY(float32, m_fVignetteMin); // ?
		PROPERTY(float32, m_fVignetteMax); // ?
		PROPERTY(SColorRGB, m_VignetteColor); // color of the vignette

		PROPERTY(float32, m_fVignetteGradientLightStrenght); // strength of light gradient at top of screen. 0.0 - 1.0
		PROPERTY(float32, m_fVignetteGradientLightSize); // how far down the light gradient reaches. 0.0 - 1.0, where 1.0 is full screen height
		PROPERTY(SColorRGB, m_vVignetteGradientLightColor); // color of the light gradient

		
		PROPERTY(bool, m_bDistortionWobbleEnabled); // enable distortion wobble effect
		PROPERTY(float32, m_fDistortionWobbleScale); // strength of wobble. 0.0 - 1.0, but can go higher with distortion issues
		PROPERTY(SVector2, m_vDistortionWobbleWaveLength); // "wavelength" of the wobble effects, both axis. 0.0 - 1.0, at 1.0 there's only a single "wave" across the screen
		PROPERTY(SVector2, m_vDistortionWobbleSpeed); // speed of the "waves" in both axis
		PROPERTY(bool, m_bDistortionWobbleUseRealTime); // use real time instead of game time for wobble animation (? idk, does nothing)


		PROPERTY(bool, m_bDistortionBarrelEnabled); // enable barrel distortion effect
		PROPERTY(float32, m_fDistortionBarrelScale); // scale of barrel distortion. 0.0 - 1.0, but can go higher with distortion issues
		PROPERTY(SVector2, m_vDistortionBarrel); // x/y distortion amounts. 0.0 - 1.0


		PROPERTY(bool, m_bGaussianBlurEnabled); // enable gaussian blur over the whole screen
		PROPERTY(float32, m_fGaussianBlurriness); // how blurry the screen is. 0.0 - 1.0; 0.1 is already plenty


		PROPERTY(bool, m_bHDREnabled); // enable HDR features
		PROPERTY(SColorRGB, m_HDRColorTint); // apply tint to the screen


		//PROPERTY(bool, m_bDoubleVisionEnabled); // idk, does nothing?


		INPUT_PIN(BlendIn);
		INPUT_PIN(BlendOut);
		INPUT_PIN(BlendInOut);
	};
#pragma endregion

protected: // API
	void BlendIn(const float32 p_fBlendInDuration = 1.0f);
	void BlendOut(const float32 p_fBlendOutDuration = 1.0f);
	void BlendInOut(const float32 p_fBlendInDuration = 1.0f, const float32 p_fBlendOutDuration = 1.0f, const float32 p_fBlendSustainDuration = 1.0f);

	/**
	 * Get wrapper to parameters entity.
	 * Set your parameters on this, then call BlendIn/BlendOut/BlendInOut on the layer to activate and deactivate.
	 */
	inline SParametersEntityBinding& GetParameters()
	{
		return m_PostfilterParameters;
	}

private:
	SLayerEntityBinding m_PostfilterLayer;
	SParametersEntityBinding m_PostfilterParameters;
};
