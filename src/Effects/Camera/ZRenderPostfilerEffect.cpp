#include "ZRenderPostfilerEffect.h"

#include <Logging.h>

#include <Glacier/ZAction.h>

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZRenderPostfilerEffect] "

static ZTemplateEntitySpawner<"[assembly:/_pro/chaosmod/custompostfilterlayer.entitytemplate].pc_entitytype"> g_CustomPostfilterLayer;

class IRenderPostfilterParametersEntity;

class ZPostfilterGraphBaseNode
{
};

class ZPostfilterBlendEntity :
	public ZEntityImpl,
	public ZPostfilterGraphBaseNode
{
public:
	TEntityRef<ZPostfilterGraphBaseNode> m_input0; // 0x20
	TEntityRef<ZPostfilterGraphBaseNode> m_input1; // 0x30
	bool m_bEnableBlendMask; // 0x40
	bool m_bEnableOverlay; // 0x41
};

class ZPostfilterLayerEntity : public ZEntityImpl
{
public:
	TEntityRef<IRenderPostfilterParametersEntity> m_parametersEntity; // 0x18
	TEntityRef<ZPostfilterGraphBaseNode> m_postfilterGraphNode; // 0x28
	TEntityRef<ZPostfilterBlendEntity> m_blendNode; // 0x38
	float32 m_fBlendInDuration; // 0x48
	float32 m_fBlendOutDuration; // 0x4C
	float32 m_fBlendSustainDuration; // 0x50
};


class ZRenderPostfilterParametersEntity;

void ZRenderPostfilerEffect::LoadResources()
{
	m_pCustomLayer = g_CustomPostfilterLayer.CreateSession();
}

void ZRenderPostfilerEffect::OnEnterScene() {}

void ZRenderPostfilerEffect::OnActivatingScene()
{
	m_rPostfilterGraphEntity = {};
	m_bPostfilterGraphEntityLoaded = true;

	const Utils::EntityFinder::SSearchParams s_Query{
		// TODO docstring false
		// TODO use search by BLU hash [modules:/zrenderpostfiltercontrollerentity.class].pc_entitytype instead of id
	   .m_nEntityId = 0x9d65a44bef250771,

	   // hopefully there's only one...
	   .m_nMaxResults = 1
	};
	const auto m_aResults = Utils::EntityFinder::FindEntities(s_Query);
	if (m_aResults.empty())
	{
		Logger::Warn(TAG "Failed to grab postfilter graph entity.");
		return;
	}

	m_rPostfilterGraphEntity = m_aResults.front();

	Start();
}

void ZRenderPostfilerEffect::OnClearScene()
{
	m_pCustomLayer = nullptr;
	m_rPostfilterGraphEntity = {};
	m_bPostfilterGraphEntityLoaded = false;
}

bool ZRenderPostfilerEffect::Available() const
{
	return IChaosEffect::Available() &&
		// if not yet loaded, report Available so that OnEnterScene gets called
		(!m_bPostfilterGraphEntityLoaded || m_rPostfilterGraphEntity) &&
		(m_pCustomLayer && m_pCustomLayer->IsAvailable());
}

void ZRenderPostfilerEffect::Start()
{
	// spawn custom postfilter layer
	auto s_rEntity = m_pCustomLayer->SpawnAs<ZPostfilterGraphBaseNode>();
	if (!s_rEntity)
	{
		return;
	}

	// insert custom layer into postfilter graph:
	// retrieve original root node
	auto s_rOriginalRootNode = m_rPostfilterGraphEntity.GetProperty<TEntityRef<ZPostfilterGraphBaseNode>>("m_blendGraphRoot").Get();
	if (!s_rOriginalRootNode)
	{
		return;
	}

	// second node
	auto s_rOriginalSecondNode = s_rOriginalRootNode.m_ref.GetProperty<TEntityRef<ZPostfilterGraphBaseNode>>("m_input0").Get();

	// set as input to our custom layer
	s_rEntity.m_ref.SetProperty("m_input0", s_rOriginalSecondNode);

	// set our custom layer as new input
	s_rOriginalRootNode.m_ref.SetProperty("m_input0", s_rEntity);
	
	// example usage of custom layer
	//s_rEntity.SetProperty("m_bFogEnabled", /*bool*/ true);
	//s_rEntity.SetProperty("m_bFogGlobal", /*bool*/ true);
	//s_rEntity.SetProperty("m_fFogDensity", /*float32*/ 0.8f);

	//s_rEntity.m_ref.SignalInputPin("BlendIn");

	// TODO test interface
	auto* s_pLayer = s_rEntity.m_ref.QueryInterface<ZPostfilterLayerEntity>();

	Logger::Debug(TAG "got custom layer entity: ", s_pLayer ? "Y" : "N");

	Logger::Debug(TAG
		"parametersentity: {:016X} \npostfiltergraphn: {:016X} \nblendnode : {:016X} \nbleninduration : {} \nblendoutduration : {} \nsustainduration : {}",
		s_pLayer->m_parametersEntity.m_ref.GetEntity()->GetType()->m_nEntityId,
		s_pLayer->m_postfilterGraphNode.m_ref.GetEntity()->GetType()->m_nEntityId,
		s_pLayer->m_blendNode.m_ref.GetEntity()->GetType()->m_nEntityId,
		s_pLayer->m_fBlendInDuration,
		s_pLayer->m_fBlendOutDuration,
		s_pLayer->m_fBlendSustainDuration
	);


	auto* s_pBlend = s_pLayer->m_blendNode.m_pInterfaceRef;

	Logger::Debug(TAG
		"blendinput0: {:016X} \nblendinput1: {:016X} \nenableblendmask: {} \nenableoverlay: {}",
		s_pBlend->m_input0.m_ref.GetEntity()->GetType()->m_nEntityId,
		s_pBlend->m_input1.m_ref.GetEntity()->GetType()->m_nEntityId,
		s_pBlend->m_bEnableBlendMask,
		s_pBlend->m_bEnableOverlay
	);
}

void ZRenderPostfilerEffect::Stop()
{
}

REGISTER_CHAOS_EFFECT(ZRenderPostfilerEffect)
