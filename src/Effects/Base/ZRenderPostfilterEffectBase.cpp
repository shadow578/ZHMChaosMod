#include "ZRenderPostfilterEffectBase.h"

#include <Logging.h>

#include "EffectRegistry.h"

#define TAG "[ZRenderPostfilterEffectBase] "

class IRenderPostfilterParametersEntity;

void ZRenderPostfilterEffectBase::OnEnterScene()
{
	m_PostfilterLayer = {};
	m_PostfilterParameters = {};

	const Utils::EntityFinder::SSearchParams s_Query{
		// we use a pre-defined ZPostFilterLayerEntity "CustomPFController".
		// that way, we can bypass having to modify the PF Graph, at the cost of only having one postfilter effect to use.
		// 
		// to find the entity id, look in entity template
		// "assembly:/_pro/effects/templates/postfilters/fx_pf_postfiltercontroller.template?/fx_pf_postfiltercontroller.entitytemplate"
		// for the ZPostFilterLayerEntity in this path:
		// "FX_PF_PostfilterController/CustomPFController/PostfilterLayerEntity11"
		// (yes, IOI got really bad at naming things here...)
	   .m_nEntityId = 0x87805acc41dd775e,

	   // hopefully there's only one...
	   .m_nMaxResults = 1
	};
	const auto m_aPostfilterEntities = Utils::EntityFinder::FindEntities(s_Query);
	if (m_aPostfilterEntities.empty())
	{
		Logger::Warn(TAG "Failed to grab CustomPFController entity.");
		return;
	}

	auto m_rPostfilterLayerEntity = m_aPostfilterEntities.front();

	// grab parameters entity from the layer
	auto s_rParametersEntity = Utils::GetProperty<TEntityRef<IRenderPostfilterParametersEntity>>(m_rPostfilterLayerEntity, "m_parametersEntity");
	if (!s_rParametersEntity)
	{
		Logger::Warn(TAG "Failed to grab parameters entity from CustomPFController.");
		return;
	}

	m_PostfilterLayer = SLayerEntityBinding(m_rPostfilterLayerEntity);
	m_PostfilterParameters = SParametersEntityBinding(s_rParametersEntity.value().m_ref);
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
	ImGui::TextUnformatted(fmt::format("Layer Entity ID: {:016X}", m_PostfilterLayer.m_rEntity.GetEntity()->GetType()->m_nEntityId).c_str());
	ImGui::TextUnformatted(fmt::format("Parameters Entity ID: {:016X}", m_PostfilterParameters.m_rEntity.GetEntity()->GetType()->m_nEntityId).c_str());
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
