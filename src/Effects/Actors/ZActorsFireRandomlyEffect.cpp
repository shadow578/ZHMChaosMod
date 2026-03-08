#include "ZActorsFireRandomlyEffect.h"

#include <Logging.h>
#include <imgui.h>

#include "Registry.h"
#include "ZConfigurationAccessor.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZActorsFireRandomlyEffect] "

void ZActorsFireRandomlyEffect::Start()
{
    SetAllActorsShootingContinuous(true);
}

void ZActorsFireRandomlyEffect::Stop()
{
    SetAllActorsShootingContinuous(false);
}

void ZActorsFireRandomlyEffect::SetAllActorsShootingContinuous(const bool p_bShooting)
{
    for (const auto& s_rActor : Utils::GetActors(false, false))
    {
        if (!Utils::IsArmed(s_rActor))
        {
            if (m_bArmAllActors && p_bShooting)
            {
                // not armed? let's change that
                // use a EL Matador here, as it caused the least issues during limited testing.
                // other guns can crash the game, esp. when equipping two-handed weapons on female actors (idk why).
                // even if a actor has a weapon, their animations will still be somewhat broken tho.
                ZRepositoryID s_ridMatador("77ecaad6-652f-480d-b365-cdf90820a5ec");
                if (!Utils::AddAndEquipWeapon(s_rActor, s_ridMatador))
                {
                    continue;
                }

                Logger::Debug(TAG "Armed previously unarmed actor {}", s_rActor.m_pInterfaceRef->GetActorName());
            }
            else
            {
                // skip unarmed actors, as it breaks their animations and they cant shoot anyway
                continue;
            }
        }

        if (auto s_Helper = GetShootAtHelperFor(s_rActor))
        {
            if (p_bShooting)
            {
                // setting m_bUnnoticed to true makes actors not blame 47 for shooting.
                // imo it'd be unfair to disable this, but idk maybe someday.
                // same for death context, anything other than DC_NOT_HERO will cause guards to blame 47
                s_Helper.m_bUnnoticed = true;
                s_Helper.m_eDeathContext = EDeathContext::eDC_NOT_HERO;

                // enable friendly fire between actors
                s_Helper.m_bAllowActorHitActor = true;

                // set fire interval
                s_Helper.m_fMinTime = 0.5f;
                s_Helper.m_fMaxTime = 1.5f;

                // parent transform to actor, locate slightly in front
                if (auto s_rTargetOpt = s_Helper.m_rTarget; s_rTargetOpt.has_value())
                {
                    auto s_rTarget = s_rTargetOpt.value();

                    if (auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(s_rActor.m_entityRef))
                    {
                        Utils::SetProperty<TEntityRef<ZSpatialEntity>>(s_rTarget.m_entityRef, "m_eidParent", s_rActorSpatial);

                        SMatrix43 s_mLocalTransform = SMatrix43();
                        s_mLocalTransform.Trans.y = -5.f; // in front
                        s_mLocalTransform.Trans.z = 1.2f; // ~chest height
                        Utils::SetProperty<SMatrix43>(s_rTarget.m_entityRef, "m_mTransform", s_mLocalTransform);
                    }
                }

                // go!
                s_Helper.StartFireContinuous();
            }
            else
            {
                s_Helper.StopFireContinuous();
            }
        }
    }
}

void ZActorsFireRandomlyEffect::LoadConfiguration(const ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::LoadConfiguration(p_pConfiguration);

    m_bArmAllActors = p_pConfiguration->GetBool("ArmAllActors", m_bArmAllActors);
}

void ZActorsFireRandomlyEffect::DrawConfigUI(ZConfigurationAccessor* p_pConfiguration)
{
    IChaosEffect::DrawConfigUI(p_pConfiguration);

    if (ImGui::Checkbox("Arm all Actors (experimental)", &m_bArmAllActors))
    {
        p_pConfiguration->SetBool("ArmAllActors", m_bArmAllActors);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Gives all Actors a weapon. Note that this is experimental and could cause issues from animations being broken to full-on game crashes.");
    }
}

REGISTER_CHAOS_EFFECT(ZActorsFireRandomlyEffect);
