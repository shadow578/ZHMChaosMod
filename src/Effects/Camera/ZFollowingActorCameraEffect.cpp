#include "ZFollowingActorCameraEffect.h"

#include <Logging.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZFollowingActorCameraEffect] "

bool ZFollowingActorCameraEffect::Available() const
{
	return ZCameraEffectBase::Available() 
		&& ZActorFollowPlayerHelperEffectBase::Available();
}

void ZFollowingActorCameraEffect::OnDrawDebugUI()
{
	ZCameraEffectBase::OnDrawDebugUI();
	ZActorFollowPlayerHelperEffectBase::OnDrawDebugUI();

	ImGui::TextUnformatted(fmt::format("Following Actor: {}", 
		m_rFollowingActor ? m_rFollowingActor.m_pInterfaceRef->GetActorName().c_str() : "None"
	).c_str());
}

void ZFollowingActorCameraEffect::Start()
{
	ZCameraEffectBase::Start();
	if (!IsEffectCameraActive())
	{
		return;
	}

	// get player location
	if (const auto s_rPlayer = SDK()->GetLocalPlayer())
	{
		if (const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef))
		{
			const auto s_vPlayerPosition = s_rPlayerSpatial.m_pInterfaceRef->GetObjectToWorldMatrix().Pos;

			// select the closest actor to the player to follow
			// limit to 100 units
			const auto& s_arActors = Utils::GetNearbyActors(
				s_vPlayerPosition,
				1,		// max count
				100.f  // max distance
			);
			if (s_arActors.size() < 1)
			{
				Logger::Error(TAG "No nearby actor found!");
				return;
			}

			MakeActorFollowingCameraPerson(s_arActors[0].first);
		}
	}
}

void ZFollowingActorCameraEffect::Stop()
{
	ZCameraEffectBase::Stop();

	// stop actor following the player
	if (m_rFollowingActor)
	{
		auto s_FollowHelper = GetFollowHelperFor(m_rFollowingActor);
		s_FollowHelper.StopFollowHitman();

		m_rFollowingActor = {};
		m_rFollowingActorHeadAttach = {};
	}
}

void ZFollowingActorCameraEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
	ZCameraEffectBase::OnFrameUpdate(p_UpdateEvent, p_fEffectTimeRemaining);

	// copy camera rotation to effect camera every frame, so player movement isn't impeded
	if (IsEffectCameraActive() && m_rFollowingActorHeadAttach)
	{
		if (auto s_rEffectCameraSpatial = TEntityRef<ZSpatialEntity>(GetEffectCameraEntity()))
		{
			if (auto s_rOriginalCameraSpatial = TEntityRef<ZSpatialEntity>(GetOriginalCameraEntity()))
			{
				const auto s_mOriginalTransform = s_rOriginalCameraSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
				const auto s_mActorHeadTransform = m_rFollowingActorHeadAttach.m_pInterfaceRef->GetObjectToWorldMatrix();

				auto s_mEffectTransform = s_rEffectCameraSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
				s_mEffectTransform.XAxis = s_mOriginalTransform.XAxis; // rotation from original camera
				s_mEffectTransform.YAxis = s_mOriginalTransform.YAxis;
				s_mEffectTransform.ZAxis = s_mOriginalTransform.ZAxis;
				s_mEffectTransform.Trans = s_mActorHeadTransform.Trans; // translation from actor head

				s_rEffectCameraSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mEffectTransform);
			}
		}
	}
}

void ZFollowingActorCameraEffect::MakeActorFollowingCameraPerson(const TEntityRef<ZActor> p_rActor)
{
	m_rFollowingActor = p_rActor;

	// start following the player
	auto s_FollowHelper = GetFollowHelperFor(m_rFollowingActor);
	if (!s_FollowHelper)
	{
		Logger::Error(TAG "Failed to get follow helper for actor!");
		return;
	}

	s_FollowHelper.m_fMinTetherRange = 2.f;
	s_FollowHelper.m_fMaxTetherRange = 5.f;

	// make actor ignore most everything so they keep following
	s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreLowNoise = true;
	s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreSillyHitman = true;
	s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreAnnoyingHitman = true;
	s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreDistractions = true;
	s_FollowHelper.m_AIModifierRoleBinding.m_bIgnoreAccidents = true;
	s_FollowHelper.m_AIModifierRoleBinding.m_bNeverSpectate = true;

	s_FollowHelper.StartFollowHitman();

	// attach camera to the actor's head
	m_rFollowingActorHeadAttach = Utils::GetActorHeadAttachEntity(p_rActor);
	if (!m_rFollowingActorHeadAttach)
	{
		Logger::Error(TAG "Failed to get head attach for actor!");
		return;
	}
}

REGISTER_CHAOS_EFFECT(ZFollowingActorCameraEffect);
