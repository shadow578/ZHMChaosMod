#include "ZOfflineVoting.h"

#include "EffectRegistry.h"
#include "Helpers/Math.h"

void ZOfflineVoting::StartVote(const std::vector<IChaosEffect*>& p_aOptions)
{
	m_aActiveVote = p_aOptions;
}

IChaosEffect* ZOfflineVoting::EndVote()
{
	if (m_aActiveVote.empty())
	{
		return nullptr;
	}

	auto* s_pResult = Math::SelectRandomElement(m_aActiveVote);
	m_aActiveVote.clear();

	return s_pResult;
}

void ZOfflineVoting::DrawOverlayUI()
{
	for (const auto& s_pEffect : m_aActiveVote)
	{
		ImGui::BulletText("%s", s_pEffect->GetDisplayName(true).c_str());
	}
}

REGISTER_VOTING_INTEGRATION(ZOfflineVoting);
