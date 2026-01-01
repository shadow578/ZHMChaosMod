#include "ZRenderPostfilerEffect.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZRenderPostfilerEffect] "

class IRenderPostfilterParametersEntity;

void ZRenderPostfilerEffect::OnEnterScene()
{
	m_rPostfilterLayerEntity = {};
	m_rPostfilterParametersEntity = {};
	m_bPostfilterLoaded = true;

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

	m_rPostfilterLayerEntity = m_aPostfilterEntities.front();

	// grab parameters entity from the layer
	auto s_rParametersEntity = m_rPostfilterLayerEntity.GetProperty<TEntityRef<IRenderPostfilterParametersEntity>>("m_parametersEntity").Get();
	if (!s_rParametersEntity)
	{
		Logger::Warn(TAG "Failed to grab parameters entity from CustomPFController.");
		return;
	}

	m_rPostfilterParametersEntity = s_rParametersEntity.m_ref;
}

void ZRenderPostfilerEffect::OnClearScene()
{
	m_rPostfilterLayerEntity = {};
	m_rPostfilterParametersEntity = {};
	m_bPostfilterLoaded = false;
}

bool ZRenderPostfilerEffect::Available() const
{
	return IChaosEffect::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
		(!m_bPostfilterLoaded || (m_rPostfilterLayerEntity && m_rPostfilterParametersEntity));
}

void ZRenderPostfilerEffect::Start()
{
	if (!m_rPostfilterLayerEntity || !m_rPostfilterParametersEntity)
	{
		return;
	}

	m_rPostfilterParametersEntity.SetProperty("m_bFogEnabled", /*bool*/ true);
	m_rPostfilterParametersEntity.SetProperty("m_bFogGlobal", /*bool*/ true);
	m_rPostfilterParametersEntity.SetProperty("m_fFogDensity", /*float32*/ 0.8f);

	m_rPostfilterLayerEntity.SignalInputPin("BlendIn");
}

void ZRenderPostfilerEffect::Stop()
{
	if (!m_rPostfilterLayerEntity || !m_rPostfilterParametersEntity)
	{
		return;
	}

	m_rPostfilterLayerEntity.SignalInputPin("BlendOut");
}

REGISTER_CHAOS_EFFECT(ZRenderPostfilerEffect)
