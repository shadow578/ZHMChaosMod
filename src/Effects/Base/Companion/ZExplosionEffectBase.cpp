#include "ZExplosionEffectBase.h"

#include <imgui.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZExplosionEffectBase] "

void ZExplosionEffectBase::LoadResources()
{
    m_pExplosionSpawner = ZTemplateEntitySpawner::Create<"[assembly:/_pro/chaosmod/explosioneffect.entitytemplate].pc_entitytype">();
}

void ZExplosionEffectBase::OnClearScene()
{
    m_pExplosionSpawner = nullptr;
}

bool ZExplosionEffectBase::Available() const
{
    return ZCompanionModDependentEffectBase::Available() && m_pExplosionSpawner && m_pExplosionSpawner->IsAvailable();
}

void ZExplosionEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pExplosionSpawner->ToString()).c_str());

    ImGui::BeginDisabled(!m_pExplosionSpawner->IsAvailable());

    if (ImGui::Button("Spawn Nearby"))
    {
        SMatrix s_mPlayerTransform;
        if (Utils::GetPlayerTransform(s_mPlayerTransform))
        {
            // ~10 forward
            const auto s_Forward = (-s_mPlayerTransform.Backward).Normalized();
            s_mPlayerTransform.Trans += s_Forward * 10.0f;

            SExplosionParams s_Params{
                .m_Position = s_mPlayerTransform,
            };
            SpawnExplosion(s_Params);
        }
    }

    ImGui::EndDisabled();
}

ZEntityRef ZExplosionEffectBase::SpawnExplosion(const SExplosionParams& p_Params)
{
    if (!m_pExplosionSpawner)
    {
        return {};
    }

    if (auto s_RootEntity = m_pExplosionSpawner->SpawnAs<ZSpatialEntity>())
    {
        s_RootEntity.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(p_Params.m_Position);

        Utils::SetProperty<float32>(s_RootEntity.m_entityRef, "m_fTimeMin", p_Params.m_fFuseTimeMin);
        Utils::SetProperty<float32>(s_RootEntity.m_entityRef, "m_fTimeMax", p_Params.m_fFuseTimeMax);
        Utils::SetProperty<float32>(s_RootEntity.m_entityRef, "m_fTargetStrength", p_Params.m_fTargetStrength);
        Utils::SetProperty<EDeathContext>(s_RootEntity.m_entityRef, "m_eDeathContext", p_Params.m_eDeathContext);
        Utils::SetProperty<EDeathType>(s_RootEntity.m_entityRef, "m_eDeathType", p_Params.m_eDeathType);

        s_RootEntity.m_entityRef.SignalInputPin("Start");

        return s_RootEntity.m_entityRef;
    }

    return {};
}
