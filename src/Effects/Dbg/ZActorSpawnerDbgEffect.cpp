#ifdef _DEBUG
#include "ZActorSpawnerDbgEffect.h"

#include <imgui.h>
#include <Logging.h>

#include <Glacier/ZHitman5.h>
#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"

#define TAG "[ZActorSpawnerDbgEffect] "

void ZActorSpawnerDbgEffect::OnDrawDebugUI()
{
    ZActorSpawnerEffectBase::OnDrawDebugUI();

    ImGui::SeparatorText("Actor Spawner");
    ImGui::BeginDisabled(!Available());

    ImGui::InputText("Actor Name", m_szActorName, 256);
    ImGui::InputText("Outfit Repository ID", m_szOutfitRepositoryID, 256);
    ImGui::InputInt("Charset Index", &m_nCharsetIndex);
    ImGui::InputInt("Outfit Variation Index", &m_nOutfitVariationIndex);

    if (ImGui::Button("Spawn Actor"))
    {
        if (const auto s_rPlayer = Utils::GetLocalPlayer())
        {
            if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                auto s_mPos = s_pPlayerSpatial->GetObjectToWorldMatrix();

                // ~10 forward
                const auto s_vForward = (-s_mPos.Backward).Normalized();
                s_mPos.Trans += s_vForward * 10.0f;

                auto s_rNewActor = SpawnActor(s_mPos, m_szActorName, m_szOutfitRepositoryID, m_nCharsetIndex, m_nOutfitVariationIndex);
                if (s_rNewActor)
                {
                    m_rTargetActor = s_rNewActor;
                }
                else
                {
                    Logger::Error(TAG "Failed to spawn actor!");
                }
            }
        }
    }
    ImGui::EndDisabled();

    ImGui::SeparatorText("ActorUtils::SetActorOutfit()");
    if (ImGui::Button("Select Nearest Actor"))
    {
        if (const auto s_rPlayer = Utils::GetLocalPlayer())
        {
            if (const auto s_pPlayerSpatial = s_rPlayer.m_entityRef.QueryInterface<ZSpatialEntity>())
            {
                const auto s_vPlayerPos = s_pPlayerSpatial->GetObjectToWorldMatrix().Trans;

                if (const auto s_aNearby = Utils::GetNearbyActors(s_vPlayerPos, 1); !s_aNearby.empty())
                {
                    m_rTargetActor = s_aNearby.front().first;
                }
            }
        }
    }
    ImGui::TextUnformatted(fmt::format("Selected Actor: {}", m_rTargetActor ? m_rTargetActor.m_pInterfaceRef->GetActorName() : "<none>").c_str());

    ImGui::BeginDisabled(!m_rTargetActor);

    ImGui::InputText("Outfit Common Name (for SetActorOutfit)", m_szOutfitCommonName, 256);
    if (ImGui::Button("SetActorOutfit()"))
    {
        Utils::SetActorOutfit(m_rTargetActor, m_szOutfitCommonName, m_nCharsetIndex, m_nOutfitVariationIndex);
    }

    ImGui::EndDisabled();
}

REGISTER_CHAOS_EFFECT(ZActorSpawnerDbgEffect);

#endif // _DEBUG
