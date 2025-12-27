#include "ZPoisonAOEDamageEffectBase.h"

#include "IPluginInterface.h"
#include <Logging.h>

#include <Glacier/ZSpatialEntity.h>

#include "Helpers/ZEntityResourceSpawner.h"

#define TAG "[ZPoisonAOEDamageEffectBase] "

static ZEntityResourceSpawner<"[assembly:/_pro/chaosmod/areaeffect_poison.entitytemplate].pc_entitytype"> g_AOECloudProp;

void ZPoisonAOEDamageEffectBase::LoadResources()
{
	m_bIsAvailable = g_AOECloudProp.IsAvailable();
}

void ZPoisonAOEDamageEffectBase::OnDrawDebugUI()
{
    static const std::map<EPoisonType, std::string> c_mPoisonTypesToNames{
        { EPoisonType::SICK, "Sick" },
        { EPoisonType::SEDATIVE, "Sedative" },
        { EPoisonType::LETHAL_SLOW, "Lethal Slow" },
        { EPoisonType::LETHAL_FAST, "Lethal Fast" },
    };

    ImGui::TextUnformatted(fmt::format("Prop: {}", g_AOECloudProp.ToString()).c_str());

    ImGui::Separator();

    ImGui::BeginDisabled(!g_AOECloudProp.IsAvailable(false));

    if (ImGui::BeginCombo("DBG Poison Type", c_mPoisonTypesToNames.at(m_eDebugPoisonType).c_str()))
    {
        for (auto [s_eType, s_sName] : c_mPoisonTypesToNames)
        {
            if (ImGui::Selectable(
                s_sName.c_str(),
                s_eType == m_eDebugPoisonType
            ))
            {
                m_eDebugPoisonType = s_eType;
            }
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Spawn DBG Nearby"))
    {
        if (const auto s_Player = SDK()->GetLocalPlayer())
        {
            if (const auto s_PlayerSpatial = s_Player.m_ref.QueryInterface<ZSpatialEntity>())
            {
                auto s_WM = s_PlayerSpatial->GetWorldMatrix();

                // ~10 forward
                const auto s_Forward = (-s_WM.Backward).Normalized();
                s_WM.Trans += s_Forward * 10.0f;

                SParams s_Params{
                    .m_Position = s_WM,
                    .m_eType = m_eDebugPoisonType
                };
                Spawn(s_Params);
            }
        }
    }

    ImGui::EndDisabled();
}

void ZPoisonAOEDamageEffectBase::Spawn(const SParams& p_Params)
{
	auto s_RootEntity = g_AOECloudProp.SpawnAs<ZSpatialEntity>();
    if (!s_RootEntity)
    {
        return;
    }

    s_RootEntity.m_pInterfaceRef->SetWorldMatrix(p_Params.m_Position);

    // set poison type
    ZEntityRef s_KeywordEntity;
    if (!GetPoisonKeywordEntity(p_Params.m_eType, s_RootEntity.m_ref, s_KeywordEntity))
    {
        Logger::Debug(TAG "Could not find poison keyword entity for type {}", static_cast<int>(p_Params.m_eType));
        return;
    }

    s_RootEntity.m_ref.SetProperty("m_rTarget", s_KeywordEntity);

    // misc. properties
    s_RootEntity.m_ref.SetProperty("m_vGlobalSize", p_Params.m_AreaSize);
    s_RootEntity.m_ref.SetProperty("m_ParticleColorRangeStart", p_Params.m_ParticleColorRangeStart);
    s_RootEntity.m_ref.SetProperty("m_ParticleColorRangeEnd", p_Params.m_ParticleColorRangeEnd);

    // trigger
    s_RootEntity.m_ref.SignalInputPin("Start");
}

bool ZPoisonAOEDamageEffectBase::GetPoisonKeywordEntity(const EPoisonType p_eType, ZEntityRef p_RootEntity, ZEntityRef& p_KeywordEntity)
{
    auto* s_pPropRTBpFactory = p_RootEntity.GetBlueprintFactory();
    if (!s_pPropRTBpFactory)
    {
        Logger::Debug(TAG "Could not get blueprint factory.");
        return false;
	}

    if (const auto idx = s_pPropRTBpFactory->GetSubEntityIndex(static_cast<int>(p_eType)); idx != -1)
    {
        if (const auto s_Ent = s_pPropRTBpFactory->GetSubEntity(p_RootEntity.m_pEntity, idx))
        {
            p_KeywordEntity = s_Ent;
            return true;
        }
    }

    return false;
}
