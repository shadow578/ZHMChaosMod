#include "ZLiveReactionCamEffect.h"

#include <Glacier/ZResourceID.h>
#include <Glacier/CompileReflection.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

static ZTemplateEntitySpawner<"[assembly:/_pro/design/templates/camera/pictureinpicture.template?/pip_camera_custom_event.entitytemplate].pc_entitytype"> g_PIPCameraProp;

void ZLiveReactionCamEffect::LoadResources()
{
	m_pPIPCameraSpawner = g_PIPCameraProp.CreateSession();
}

void ZLiveReactionCamEffect::OnClearScene()
{
	m_pPIPCameraSpawner = nullptr;
}

bool ZLiveReactionCamEffect::Available() const
{
	return IChaosEffect::Available() &&
		m_pPIPCameraSpawner &&
		m_pPIPCameraSpawner->IsAvailable();
}

void ZLiveReactionCamEffect::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("Prop: {}", m_pPIPCameraSpawner->ToString()).c_str());
}

void ZLiveReactionCamEffect::Start()
{
	// avoid multiple
	if (m_rPIPCameraEntity)
	{
		Stop();
	}

	// spawn pip camera
	if (auto s_rTargetHead = GetPlayerHeadAttachEntity())
	{
		SpawnLiveReactionCam(s_rTargetHead, ResId<"">);
	}

	// activate the PIP camera
	if (m_rPIPCameraEntity)
	{
		m_rPIPCameraEntity.SignalInputPin("Activate");
	}
}

void ZLiveReactionCamEffect::Stop()
{
	if (!m_rPIPCameraEntity)
	{
		return;
	}

	// deactivate PIP before despawning
	m_rPIPCameraEntity.SignalInputPin("Deactivate");

	m_pPIPCameraSpawner->Despawn(m_rPIPCameraEntity);
	m_rPIPCameraEntity = {};
}

TEntityRef<ZSpatialEntity> ZLiveReactionCamEffect::GetRandomActorHeadAttachEntity()
{
	constexpr uint64_t c_nHeadEntityId = 0x5f46597848b36b38; // "HEAD"

	const auto s_pActor = Utils::GetRandomActor(true);
	if (!s_pActor)
	{
		return {};
	}

	ZEntityRef s_rActor;
	s_pActor->GetID(s_rActor);
	if (!s_rActor)
	{
		return {};
	}

	if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(s_rActor))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(c_nHeadEntityId); s_nIdx != -1)
		{
			if (auto* s_pHead = s_pBlueprint->GetSubEntity(s_rActor.m_pEntity, s_nIdx); s_pHead != nullptr)
			{
				return TEntityRef<ZSpatialEntity>(ZEntityRef(s_pHead));
			}
		}
	}

	return {};
}

TEntityRef<ZSpatialEntity> ZLiveReactionCamEffect::GetPlayerHeadAttachEntity()
{
	constexpr uint64_t c_nHeadEntityId = 0x0ff5798a35665af2; // "HEAD"

	const auto s_rPlayer = SDK()->GetLocalPlayer();
	if (!s_rPlayer)
	{
		return {};
	}

	if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(s_rPlayer.m_ref))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(c_nHeadEntityId); s_nIdx != -1)
		{
			if (auto* s_pHead = s_pBlueprint->GetSubEntity(s_rPlayer.m_ref.m_pEntity, s_nIdx); s_pHead != nullptr)
			{
				return TEntityRef<ZSpatialEntity>(ZEntityRef(s_pHead));
			}
		}
	}

	return {};
}

void ZLiveReactionCamEffect::SpawnLiveReactionCam(TEntityRef<ZSpatialEntity> p_rTargetHead, const ZRuntimeResourceID p_HudMessageId)
{
	if (!p_rTargetHead)
	{
		return;
	}

	// get PIP camera
	m_rPIPCameraEntity = m_pPIPCameraSpawner->Spawn();
	if (!m_rPIPCameraEntity)
	{
		return;
	}

	// move relative to head, with a local offset and rotation
	// QNE offset:
	// P: 0.055 0.700 0.000
	// R: 90.00 90.00 0.000
	//
	// x offset (0.055) needed since head pivot is roughly at the neck, but we want head center
	// y offset (0.700) to be a bit in front of the face
	m_rPIPCameraEntity.SetProperty("m_eidParent", /*TEntityRef<SpatialEntity>*/ p_rTargetHead);

	SMatrix43 s_mLocalTransform;
	s_mLocalTransform.XAxis = { 0.000f, -0.000f,  1.000f };
	s_mLocalTransform.YAxis = { 1.000f,  0.000f, -0.000f };
	s_mLocalTransform.ZAxis = { -0.000f, 1.000f,  0.000f };
	s_mLocalTransform.Trans = { 0.055f,  0.700f,  0.000 };
	m_rPIPCameraEntity.SetProperty("m_mTransform", /*SMatrix43*/ s_mLocalTransform);

	// prevent PIP view from automatically closing after timer (we do it manually)
	m_rPIPCameraEntity.SetProperty("m_bValueusepiptimer", /*bool*/ false);

	// mess with the camera properties to do a funny
	m_rPIPCameraEntity.SetProperty("m_bAutoAspect", /*bool*/ false); // manual aspect
	m_rPIPCameraEntity.SetProperty("m_fAspectWByH", /*float32*/ 0.8f); // stretch horizontally
	m_rPIPCameraEntity.SetProperty("m_fFovYDeg", /*float32*/ 20.0f); // really zoom in on the face

	// the PIP view needs a HUD message, otherwise only the camera feed shows
	//[assembly:/localization/hitman6/conversations/ui/pro/hud_messages.sweetmenutext?/egame_text_sl_bodyfound.sweetline].pc_sweetline
	const auto s_rid = ZRuntimeResourceID::FromString("[assembly:/localization/chaosmod/ui/hud_pip_livereaction/player.sweetline].pc_sweetline");


	static ZTemplateEntitySpawner<"[assembly:/localization/chaosmod/ui/hud_pip_livereaction/player.sweetline].pc_sweetline"> g_slldr;
	m_rPIPCameraEntity.SetProperty("m_rHUDMessagepip", /*TResourcePtr (??)*/ s_rid);

	Logger::Debug("m_rHUDMessagepip= {:08x}", g_slldr.CreateSession()->GetResourceID().GetID());


	// by default, the PIP is automatically closed when looking at it (via look-at trigger)
	// there's no direct way to disable this, but we can manipulate the look-at trigger to effectively never trigger
	if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(m_rPIPCameraEntity))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x2c2b10f742467d79); s_nIdx != -1)
		{
			if (auto* s_pLookAtTrigger = s_pBlueprint->GetSubEntity(m_rPIPCameraEntity.m_pEntity, s_nIdx); s_pLookAtTrigger != nullptr)
			{
				auto s_rLookAtTrigger = ZEntityRef(s_pLookAtTrigger);

				s_rLookAtTrigger.SetProperty("m_nTriggerCount", /*int32*/ 0);
				s_rLookAtTrigger.SetProperty("m_fTriggerDelay", /*float32*/ 9999.0f);
			}
		}
	}
}

REGISTER_CHAOS_EFFECT(ZLiveReactionCamEffect);
