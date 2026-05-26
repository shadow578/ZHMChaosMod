#include "ZActorDanceGroupEffect.h"

#include <cmath>

#include <Logging.h>
#include <imgui.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/Math.h"

#define TAG "[ZActorDanceGroupEffect] "

void ZActorDanceGroupEffect::LoadResources()
{
    ZActorSpawnerEffectBase::LoadResources();
    ZActorActLibraryEffectBase::LoadResources();
}

void ZActorDanceGroupEffect::OnClearScene()
{
    ZActorSpawnerEffectBase::OnClearScene();
    ZActorActLibraryEffectBase::OnClearScene();
}

bool ZActorDanceGroupEffect::Available() const
{
    return ZActorSpawnerEffectBase::Available() && ZActorActLibraryEffectBase::Available();
}

void ZActorDanceGroupEffect::Start()
{

    if (const auto s_rPlayer = Utils::GetLocalPlayer())
    {
        if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
        {
            auto s_mPos = s_pPlayerSpatial->GetObjectToWorldMatrix();
            SpawnAt(s_mPos);
        }
    }
}

void ZActorDanceGroupEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    if (m_fRemainingTimeToActActivation > 0.f && !m_aDancersPendingActivation.empty())
    {
        m_fRemainingTimeToActActivation -= p_fDeltaTime;

        if (m_fRemainingTimeToActActivation <= 0.f)
        {
            for (const auto& s_rDancer : m_aDancersPendingActivation)
            {
                if (!s_rDancer)
                {
                    continue;
                }

                if (auto s_Binding = GetLambicDanceBinding(s_rDancer))
                {
                    s_Binding.Start();
                }
            }

            m_aDancersPendingActivation.clear();
            m_fRemainingTimeToActActivation = 0.f;
        }
    }
}

void ZActorDanceGroupEffect::SpawnAt(const SMatrix& p_mCenter)
{
    // activation of the act is delayed via OnSlowUpdate.
    // when the act is started right away, the actor enters a weird state where no
    // animations play at all.
    m_aDancersPendingActivation.clear();
    m_fRemainingTimeToActActivation = m_fTimeDelayToActActivation;

    const auto s_aPositions = GetSpawnPositions(p_mCenter.Pos);
    for (const auto& s_mPos : s_aPositions)
    {
        const auto s_rActor = SpawnActor(s_mPos, "Dancer", "443071d5-f833-4d9f-b332-8e8c8d05b446"); // CM_TheConstant_M via Companion Mod
        if (!s_rActor)
        {
            Logger::Error(TAG "Failed to spawn dancer actor!");
            continue;
        }

        auto s_Binding = GetLambicDanceBinding(s_rActor);
        if (!s_Binding)
        {
            Logger::Error(TAG "Failed to get act binding for dancer actor!");
            continue;
        }

        s_Binding.m_MovementType = ZActBehaviorEntity_EMovementType::MT_WALK;

        // set waypoint location right away, in case the actor moves during the activation delay.
        if (const auto s_rActSpatial = s_Binding.QuerySpatial())
        {
            s_rActSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mPos);
        }

        m_aDancersPendingActivation.emplace_back(s_rActor);
    }
}

std::vector<SMatrix> ZActorDanceGroupEffect::GetSpawnPositions(const float4& p_vCenter) const
{
    std::vector<SMatrix> s_aPositions;

    constexpr float c_fTwoPi = 2.0f * 3.1415f;

    for (int i = 0; i < m_nCount; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(m_nCount);
        const float s_fAngle = t * c_fTwoPi;

        SMatrix s_mPos = SMatrix();
        const float4 s_vPos = p_vCenter + float4(std::cos(s_fAngle) * m_fRadius, std::sin(s_fAngle) * m_fRadius, 0.f, 0.f);
        s_mPos.Pos = s_vPos;

        // rotate so forward (XY) points toward the center
        const float s_fDx = p_vCenter.x - s_vPos.x;
        const float s_fDy = p_vCenter.y - s_vPos.y;
        const float s_fLen = std::sqrt(s_fDx * s_fDx + s_fDy * s_fDy);
        if (s_fLen > 1e-6f)
        {
            const float s_fx = s_fDx / s_fLen;
            const float s_fy = s_fDy / s_fLen;

            // forward toward center, right is perpendicular, up is world Z
            // Assume SMatrix has Forward, Right, Up float4 members
            s_mPos.Backward = -float4(s_fx, s_fy, 0.f, 0.f);
            s_mPos.Right = float4(-s_fy, s_fx, 0.f, 0.f);
            s_mPos.Up = float4(0.f, 0.f, 1.f, 0.f);
        }
        s_aPositions.emplace_back(s_mPos);
    }

    return s_aPositions;
}

void ZActorDanceGroupEffect::OnDrawDebugUI()
{
    ImGuiEx::DragFloat("Delay to Act Activation after Spawn", &m_fTimeDelayToActActivation, 0.f, 10.f);

    ImGui::SeparatorText("ZActorSpawnerEffectBase");
    ZActorSpawnerEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("ZActorActLibraryEffectBase");
    ZActorActLibraryEffectBase::OnDrawDebugUI();
}

void ZActorDanceGroupEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_nCount = Math::Clamp(static_cast<int>(p_pConfiguration->GetInt("Count", m_nCount)), 1, 10);
    m_fRadius = p_pConfiguration->GetFloat("Radius", m_fRadius);
}

void ZActorDanceGroupEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGui::InputInt("Count", &m_nCount, 1, 1))
    {
        m_nCount = Math::Clamp(m_nCount, 1, 10);
        p_pConfiguration->SetInt("Count", m_nCount);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Number of dancers to spawn.");
    }

    if (ImGuiEx::DragFloat("Radius", &m_fRadius, 2.f, 50.f))
    {
        p_pConfiguration->SetDouble("Radius", m_fRadius);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Radius of the circle on which the dancers will spawn.");
    }
}

REGISTER_CHAOS_EFFECT(ZActorDanceGroupEffect);
