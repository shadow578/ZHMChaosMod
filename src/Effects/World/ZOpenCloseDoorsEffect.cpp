#include "ZOpenCloseDoorsEffect.h"

#include "EffectRegistry.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/Math.h"

void ZOpenCloseDoorsEffect::OnDrawDebugUI()
{
	ImGuiEx::DragFloat("Interval", &m_fInterval, 0.1f, 10.0f);

	ImGui::SeparatorText("ZDoorsEffectBase");
	ZDoorsEffectBase::OnDrawDebugUI();
}

void ZOpenCloseDoorsEffect::Start()
{
	m_bActive = true;
	m_fIntervalTimer = 0.0f;
	m_bNextOpenState = true;
}

void ZOpenCloseDoorsEffect::Stop()
{
	m_bActive = false;
}

void ZOpenCloseDoorsEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
	if (!m_bActive)
	{
		return;
	}

	m_fIntervalTimer += p_fDeltaTime;
	if (m_fIntervalTimer < m_fInterval)
	{
		return;
	}

	m_fIntervalTimer = 0.0f;

	auto& s_aDoors = GetDoors();
	for (auto& s_rDoor : s_aDoors)
	{
		if (m_bNextOpenState)
		{
			if (Math::GetRandomBool(0.5f))
			{
				s_rDoor.OpenDoorIn();
			}
			else
			{
				s_rDoor.OpenDoorOut();
			}
		}
		else
		{
			s_rDoor.CloseDoor();
		}
	}

	m_bNextOpenState = !m_bNextOpenState;
}

REGISTER_CHAOS_EFFECT(ZOpenCloseDoorsEffect);
