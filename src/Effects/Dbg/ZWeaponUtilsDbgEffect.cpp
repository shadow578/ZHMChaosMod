#ifdef _DEBUG
#include "ZWeaponUtilsDbgEffect.h"

#include <imgui.h>

#include <Glacier/ZHitman5.h>
#include <Glacier/ZSpatialEntity.h>

#include "Registry.h"
#include "Helpers/Math.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/Repository/ZHMRepositoryHelper.h"

#define TAG "[ZWeaponUtilsDbgEffect] "

void ZWeaponUtilsDbgEffect::OnDrawDebugUI()
{
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

    if (!m_rTargetActor)
    {
        return;
    }

    const auto s_rMainWeapon = Utils::GetMainWeapon(m_rTargetActor);
    if (s_rMainWeapon)
    {
        ImGui::TextUnformatted(fmt::format("Main Weapon: {} ({})", s_rMainWeapon.m_pInterfaceRef->m_pItemConfigDescriptor->m_sTitle.c_str(), static_cast<int>(s_rMainWeapon.m_pInterfaceRef->m_WeaponType)).c_str());
    }
    else
    {
        ImGui::TextUnformatted("No Main Weapon");
    }

    ImGui::TextUnformatted(fmt::format("Is Armed?: {}", Utils::IsArmed(m_rTargetActor) ? "true" : "false").c_str());

    if (ImGui::Button("Give Random Weapon"))
    {
        std::vector<ZRepositoryID> s_aWeaponIds;
        for (auto& s_ridWeapon : ZHMRepositoryHelper::GetInstance().GetEntryIdsByType(ZHMRepositoryHelper::EEntryType::Weapon))
        {
            s_aWeaponIds.push_back(s_ridWeapon);
        }

        const auto s_ridRandomWeapon = Math::SelectRandomElement(s_aWeaponIds);
        Logger::Info(TAG, "Giving weapon RID= {}", s_ridRandomWeapon.ToString().c_str());

        if (!Utils::AddAndEquipWeapon(m_rTargetActor, s_ridRandomWeapon))
        {
            Logger::Error(TAG, "Failed to give weapon");
        }
    }

    if (ImGui::Button("Give EL Matador"))
    {
        ZRepositoryID s_ridMatador("77ecaad6-652f-480d-b365-cdf90820a5ec");
        if (!Utils::AddAndEquipWeapon(m_rTargetActor, s_ridMatador))
        {
            Logger::Error(TAG, "Failed to give Matador");
        }
    }

    if (ImGui::Button("Holster"))
    {
        auto s_WeaponControl = GetShootAtHelperFor(m_rTargetActor);
        s_WeaponControl.Holster();
    }

    ImGui::SameLine();

    if (ImGui::Button("Unholster"))
    {
        auto s_WeaponControl = GetShootAtHelperFor(m_rTargetActor);
        s_WeaponControl.Unholster();
    }

    if (ImGui::Button("Fire"))
    {
        auto s_WeaponControl = GetShootAtHelperFor(m_rTargetActor);
        s_WeaponControl.m_bAllowActorHitActor = true;
        s_WeaponControl.Fire();
    }

    ImGui::SameLine();

    if (ImGui::Button("Start Cont. Fire"))
    {
        auto s_WeaponControl = GetShootAtHelperFor(m_rTargetActor);
        s_WeaponControl.m_bAllowActorHitActor = true;
        s_WeaponControl.StartFireContinuous();
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop Cont. Fire"))
    {
        auto s_WeaponControl = GetShootAtHelperFor(m_rTargetActor);
        s_WeaponControl.StopFireContinuous();
    }
}

REGISTER_CHAOS_EFFECT(ZWeaponUtilsDbgEffect);

#endif // _DEBUG
