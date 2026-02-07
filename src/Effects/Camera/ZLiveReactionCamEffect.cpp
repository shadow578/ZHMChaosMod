#include "ZLiveReactionCamEffect.h"

#include <Glacier/ZResourceID.h>
#include <Glacier/CompileReflection.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

void ZLiveReactionCamEffect::LoadResources()
{
	m_pPIPCameraSpawner = ZTemplateEntitySpawner::Create<"[assembly:/_pro/design/templates/camera/pictureinpicture.template?/pip_camera_custom_event.entitytemplate].pc_entitytype">();

	if (m_bTargetPlayer)
	{
		m_pHudMessageResource = ZResourceProvider::Create<"[assembly:/_pro/chaosmod/localization/ui/hud_pip_livereaction/player.sweetline].pc_sweetline">();
	}
	else
	{
		m_pHudMessageResource = ZResourceProvider::Create<"[assembly:/_pro/chaosmod/localization/ui/hud_pip_livereaction/actor.sweetline].pc_sweetline">();
	}
}

void ZLiveReactionCamEffect::OnClearScene()
{
	m_pPIPCameraSpawner = nullptr;
	m_pHudMessageResource = nullptr;
}

bool ZLiveReactionCamEffect::Available() const
{
	return IChaosEffect::Available() &&
				 m_pPIPCameraSpawner && m_pPIPCameraSpawner->IsAvailable() &&
				 m_pHudMessageResource && m_pHudMessageResource->IsAvailable();
}

void ZLiveReactionCamEffect::OnDrawDebugUI()
{
	ImGui::TextUnformatted(fmt::format("PIP Cam: {}", m_pPIPCameraSpawner->ToString()).c_str());
	ImGui::TextUnformatted(fmt::format("Sweetline: {}", m_pHudMessageResource->ToString()).c_str());
}

bool ZLiveReactionCamEffect::IsCompatibleWith(const IChaosEffect *p_pOther) const
{
	return IChaosEffect::IsCompatibleWith(p_pOther) && !Utils::IsInstanceOf<ZLiveReactionCamEffect>(p_pOther);
}

void ZLiveReactionCamEffect::Start()
{
	// avoid multiple
	if (m_rPIPCameraEntity)
	{
		Stop();
	}

	// get head of target (actor or player)
	auto s_rTargetHead = m_bTargetPlayer ? GetPlayerHeadAttachEntity() : GetRandomActorHeadAttachEntity();
	if (!s_rTargetHead)
	{
		return;
	}

	SpawnLiveReactionCam(s_rTargetHead, m_pHudMessageResource->GetResourceID(), ZPIPMessageEntity_EIcon::Target);

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

	if (auto *s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(s_rActor))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(c_nHeadEntityId); s_nIdx != -1)
		{
			if (auto *s_pHead = s_pBlueprint->GetSubEntity(s_rActor.m_pObj, s_nIdx); s_pHead != nullptr)
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

	if (auto *s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(s_rPlayer.m_entityRef))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(c_nHeadEntityId); s_nIdx != -1)
		{
			if (auto *s_pHead = s_pBlueprint->GetSubEntity(s_rPlayer.m_entityRef.m_pObj, s_nIdx); s_pHead != nullptr)
			{
				return TEntityRef<ZSpatialEntity>(ZEntityRef(s_pHead));
			}
		}
	}

	return {};
}

void ZLiveReactionCamEffect::SpawnLiveReactionCam(TEntityRef<ZSpatialEntity> p_rTargetHead, const ZRuntimeResourceID p_HudMessageId, const ZPIPMessageEntity_EIcon p_eIcon)
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
	Utils::SetProperty<TEntityRef<ZSpatialEntity>>(m_rPIPCameraEntity, "m_eidParent", p_rTargetHead);

	SMatrix43 s_mLocalTransform;
	s_mLocalTransform.XAxis = {0.000f, -0.000f, 1.000f};
	s_mLocalTransform.YAxis = {1.000f, 0.000f, -0.000f};
	s_mLocalTransform.ZAxis = {-0.000f, 1.000f, 0.000f};
	s_mLocalTransform.Trans = {0.055f, 0.700f, 0.000};
	Utils::SetProperty<SMatrix43>(m_rPIPCameraEntity, "m_mTransform", s_mLocalTransform);

	// prevent PIP view from automatically closing after timer (we do it manually)
	Utils::SetProperty<bool>(m_rPIPCameraEntity, "m_bValueusepiptimer", false);

	// take highes PIP priority so it overrides other PIP cameras
	Utils::SetProperty<int32>(m_rPIPCameraEntity, "m_nValuePiPPriority", 9999);

	// mess with the camera properties to do a funny
	Utils::SetProperty<bool>(m_rPIPCameraEntity, "m_bAutoAspect", false);		// manual aspect
	Utils::SetProperty<float32>(m_rPIPCameraEntity, "m_fAspectWByH", 0.8f); // stretch horizontally
	Utils::SetProperty<float32>(m_rPIPCameraEntity, "m_fFovYDeg", 20.0f);		// really zoom in on the face

	// the PIP view needs a HUD message, otherwise only the camera feed shows
	Utils::SetProperty<ZRuntimeResourceID>(m_rPIPCameraEntity, "m_rHUDMessagepip", p_HudMessageId);

	// set icon
	Utils::SetProperty<ZPIPMessageEntity_EIcon>(m_rPIPCameraEntity, "m_eIcon", p_eIcon);

	// by default, the PIP is automatically closed when looking at it (via look-at trigger)
	// there's no direct way to disable this, but we can manipulate the look-at trigger to effectively never trigger
	if (auto *s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(m_rPIPCameraEntity))
	{
		if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x2c2b10f742467d79); s_nIdx != -1)
		{
			if (auto *s_pLookAtTrigger = s_pBlueprint->GetSubEntity(m_rPIPCameraEntity.m_pObj, s_nIdx); s_pLookAtTrigger != nullptr)
			{
				auto s_rLookAtTrigger = ZEntityRef(s_pLookAtTrigger);

				Utils::SetProperty<int32>(s_rLookAtTrigger, "m_nTriggerCount", 0);
				Utils::SetProperty<float32>(s_rLookAtTrigger, "m_fTriggerDelay", 9999.0f);
			}
		}
	}
}

REGISTER_CHAOS_EFFECT_PARAM(Actor, ZLiveReactionCamEffect, false);
REGISTER_CHAOS_EFFECT_PARAM(Player, ZLiveReactionCamEffect, true);
