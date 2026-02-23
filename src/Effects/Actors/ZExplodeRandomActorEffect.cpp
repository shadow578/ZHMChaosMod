#include "ZExplodeRandomActorEffect.h"

#include <Glacier/ZSpatialEntity.h>

#include "EffectRegistry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZExplodeRandomActorEffect] "

void ZExplodeRandomActorEffect::Start()
{
    m_rLastTarget = Utils::GetRandomActor(true);
    if (!m_rLastTarget)
    {
        Logger::Warn(TAG "No actor found to explode!");
        return;
    }

    const auto *s_pActorSpatial = m_rLastTarget.m_entityRef.QueryInterface<ZSpatialEntity>();
    if (!s_pActorSpatial)
    {
        return;
    }

    // offset a bit upwards so it doesn't clip into the ground
    m_LastTargetPos = s_pActorSpatial->GetObjectToWorldMatrix();
    m_LastTargetPos.Trans.z += 0.25f;

    SExplosionParams s_ExplosionParams{
        .m_Position = m_LastTargetPos,
    };
    SpawnExplosion(s_ExplosionParams);
}

void ZExplodeRandomActorEffect::OnDrawDebugUI()
{
    std::string s_LastTargetName = "<null>";
    if (m_rLastTarget)
    {
        s_LastTargetName = m_rLastTarget.m_pInterfaceRef->m_sActorName;
    }

    ImGui::TextUnformatted(fmt::format("Last Target: {}", s_LastTargetName).c_str());

    if (ImGui::Button("Teleport to last site"))
    {
        Utils::TeleportPlayerTo(m_LastTargetPos);
    }

    ImGui::SeparatorText("ZExplosionEffectBase");
    ZExplosionEffectBase::OnDrawDebugUI();
}

REGISTER_CHAOS_EFFECT(ZExplodeRandomActorEffect)
