#include "ZRenderPostfilterEffectBase.h"

#include <Logging.h>

#include "EffectRegistry.h"

#define TAG "[ZRenderPostfilterEffectBase] "

void ZRenderPostfilterEffectBase::OnEnterScene()
{
	m_PostfilterLayer = {};
	m_PostfilterParameters = {};

	const auto &s_rPostfilterLayerEntity = Utils::ZEntityFinder()
																						 // we use a pre-defined ZPostFilterLayerEntity "CustomPFController".
																						 // that way, we can bypass having to modify the PF Graph, at the cost of only having one postfilter effect to use.
																						 //
																						 // to find the entity id, look in entity template
																						 // "assembly:/_pro/effects/templates/postfilters/fx_pf_postfiltercontroller.template?/fx_pf_postfiltercontroller.entitytemplate"
																						 // for the ZPostFilterLayerEntity in this path:
																						 // "FX_PF_PostfilterController/CustomPFController/PostfilterLayerEntity11"
																						 // (yes, IOI got really bad at naming things here...)
																						 .EntityID(0x87805acc41dd775e)

																						 // there should only be one
																						 .FindFirst();

	m_PostfilterLayer = SPostfilterLayerEntityBinding(s_rPostfilterLayerEntity);
	if (!m_PostfilterLayer)
	{
		m_PostfilterLayer = {};
		return;
	}

	// grab parameters entity from the layer
	m_PostfilterParameters = SRenderPostfilterParametersEntityBinding(m_PostfilterLayer.m_parametersEntity.value().m_entityRef);
	if (!m_PostfilterParameters)
	{
		Logger::Warn(TAG "Failed to grab parameters entity from CustomPFController.");
		m_PostfilterLayer = {};
		m_PostfilterParameters = {};
		return;
	}
}

void ZRenderPostfilterEffectBase::OnClearScene()
{
	m_PostfilterLayer = {};
	m_PostfilterParameters = {};
}

bool ZRenderPostfilterEffectBase::Available() const
{
	return IChaosEffect::Available() &&
				 m_PostfilterLayer &&
				 m_PostfilterParameters;
}

void ZRenderPostfilterEffectBase::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("Layer Entity ID: {:016X}", m_PostfilterLayer.m_rEntity.GetEntity()->GetType()->m_nEntityID).c_str());
	ImGui::TextUnformatted(fmt::format("Parameters Entity ID: {:016X}", m_PostfilterParameters.m_rEntity.GetEntity()->GetType()->m_nEntityID).c_str());
}

void ZRenderPostfilterEffectBase::BlendIn(const float32 p_fBlendInDuration)
{
	if (!m_PostfilterLayer)
	{
		return;
	}

	m_PostfilterLayer.m_fBlendInDuration = p_fBlendInDuration;
	m_PostfilterLayer.BlendIn();
}

void ZRenderPostfilterEffectBase::BlendOut(const float32 p_fBlendOutDuration)
{
	if (!m_PostfilterLayer)
	{
		return;
	}

	m_PostfilterLayer.m_fBlendOutDuration = p_fBlendOutDuration;
	m_PostfilterLayer.BlendOut();
}

void ZRenderPostfilterEffectBase::BlendInOut(const float32 p_fBlendInDuration, const float32 p_fBlendOutDuration, const float32 p_fBlendSustainDuration)
{
	if (!m_PostfilterLayer)
	{
		return;
	}

	m_PostfilterLayer.m_fBlendInDuration = p_fBlendInDuration;
	m_PostfilterLayer.m_fBlendOutDuration = p_fBlendOutDuration;
	m_PostfilterLayer.m_fBlendSustainDuration = p_fBlendSustainDuration;

	m_PostfilterLayer.BlendInOut();
}
