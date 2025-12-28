#include "ZExplosionEffectBase.h"

#include "IPluginInterface.h"

#include <Glacier/ZSpatialEntity.h>

#define TAG "[ZExplosionEffectBase] "

static ZTemplateEntitySpawner<"[assembly:/_pro/chaosmod/explosioneffect.entitytemplate].pc_entitytype"> g_ExplosionProp;

void ZExplosionEffectBase::LoadResources()
{
    m_pSpawnerSession = g_ExplosionProp.CreateSession();
}

void ZExplosionEffectBase::OnClearScene()
{
    m_pSpawnerSession = nullptr;
}

bool ZExplosionEffectBase::Available() const
{
    return IChaosEffect::Available() &&
        m_pSpawnerSession &&
        m_pSpawnerSession->IsAvailable();
}

void ZExplosionEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Prop: {}", m_pSpawnerSession->ToString()).c_str());

    ImGui::BeginDisabled(!m_pSpawnerSession->IsAvailable());

    if (ImGui::Button("Spawn Nearby"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

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

void ZExplosionEffectBase::SpawnExplosion(const SExplosionParams& p_Params)
{
    if (!m_pSpawnerSession)
    {
        return;
    }

    if (auto s_RootEntity = m_pSpawnerSession->SpawnAs<ZSpatialEntity>())
    {
        s_RootEntity.m_pInterfaceRef->SetWorldMatrix(p_Params.m_Position);
        s_RootEntity.m_ref.SetProperty("m_fTimeMin", p_Params.m_fFuseTimeMin);
        s_RootEntity.m_ref.SetProperty("m_fTimeMax", p_Params.m_fFuseTimeMax);
        s_RootEntity.m_ref.SetProperty("m_fTargetStrength", p_Params.m_fTargetStrength);
        s_RootEntity.m_ref.SetProperty("m_eDeathContext", p_Params.m_eDeathContext);
        s_RootEntity.m_ref.SetProperty("m_eDeathType", p_Params.m_eDeathContext);

        s_RootEntity.m_ref.SignalInputPin("Start");
    }
}
