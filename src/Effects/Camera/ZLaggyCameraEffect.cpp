#include "ZLaggyCameraEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/Math.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/EntityUtils.h"

void ZLaggyCameraEffect::Start()
{
    while (!m_qTransformHistory.empty())
    {
        m_qTransformHistory.pop();
	}

    ZCameraEffectBase::Start();
}

void ZLaggyCameraEffect::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format(
        "Current Queue Size: {}",
        m_qTransformHistory.size()
    ).c_str());

	ImGui::DragInt("Delay Frames", &m_nDelayFrames, 1.0f, 0, 120);
	ImGui::DragFloat("Apply Percent", &m_fTransformApplyPercent, 1.0f, 0.0f, 1.0f);

    ImGui::SeparatorText("ZCameraEffectBase");
    ZCameraEffectBase::OnDrawDebugUI();
}

void ZLaggyCameraEffect::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent, const float32 p_fEffectTimeRemaining)
{
    if (!IsEffectCameraActive())
    {
        return;
    }

    // get references to all involved spatial entities
    auto* s_pOriginalCameraSpatialEntity = GetOriginalCameraEntity().QueryInterface<ZSpatialEntity>();
    auto* s_pCameraSpatialEntity = GetEffectCameraEntity().QueryInterface<ZSpatialEntity>();
    if (!s_pOriginalCameraSpatialEntity || !s_pCameraSpatialEntity)
    {
        Stop();
        return;
    }

    // push current transform
    const auto s_mOriginalCameraTransform = s_pOriginalCameraSpatialEntity->GetObjectToWorldMatrix();
	m_qTransformHistory.push(s_mOriginalCameraTransform);

	// if the target queue size is reached, pop the queue to maintain the delay
    while (m_qTransformHistory.size() > (m_nDelayFrames + 1))
    {
	    m_qTransformHistory.pop();
    }

    // get transform and apply to camera with chance percent
    const auto s_mTargetTransform = m_qTransformHistory.front();
    if (Math::GetRandomBool(m_fTransformApplyPercent))
    {
        s_pCameraSpatialEntity->SetObjectToWorldMatrixFromEditor(s_mTargetTransform);
    }
}

REGISTER_CHAOS_EFFECT(ZLaggyCameraEffect)