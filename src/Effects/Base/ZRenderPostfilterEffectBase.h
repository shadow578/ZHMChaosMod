#pragma once
#include "IChaosEffect.h"

#include <Glacier/ZEntity.h>

#include "Helpers/EntityUtils.h"
#include "Helpers/EntityBindings/SPostfilterLayerEntityBinding.h"
#include "Helpers/EntityBindings/SRenderPostfilterParametersEntityBinding.h"

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

protected: // API
	void BlendIn(const float32 p_fBlendInDuration = 1.0f);
	void BlendOut(const float32 p_fBlendOutDuration = 1.0f);
	void BlendInOut(const float32 p_fBlendInDuration = 1.0f, const float32 p_fBlendOutDuration = 1.0f, const float32 p_fBlendSustainDuration = 1.0f);

	/**
	 * Get wrapper to parameters entity.
	 * Set your parameters on this, then call BlendIn/BlendOut/BlendInOut on the layer to activate and deactivate.
	 */
	inline SRenderPostfilterParametersEntityBinding &GetParameters()
	{
		return m_PostfilterParameters;
	}

private:
	SPostfilterLayerEntityBinding m_PostfilterLayer;
	SRenderPostfilterParametersEntityBinding m_PostfilterParameters;
};
