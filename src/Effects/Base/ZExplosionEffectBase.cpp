#include "ZExplosionEffectBase.h"

#include "IPluginInterface.h"

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/EntityUtils.h"

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
    return ZCompanionModDepedentEffectBase::Available() &&
           m_pExplosionSpawner &&
           m_pExplosionSpawner->IsAvailable();
}

void ZExplosionEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pExplosionSpawner->ToString()).c_str());

    ImGui::BeginDisabled(!m_pExplosionSpawner->IsAvailable());

    if (ImGui::Button("Spawn Nearby"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetObjectToWorldMatrix();

                // ~10 forward
                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 10.0f;

                SExplosionParams s_Params{
                    .m_Position = s_WM,
                };
                SpawnExplosion(s_Params);
            }
        }
    }

    ImGui::EndDisabled();
}

ZEntityRef ZExplosionEffectBase::SpawnExplosion(const SExplosionParams &p_Params)
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
