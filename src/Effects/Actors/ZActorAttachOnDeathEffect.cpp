#include "ZActorAttachOnDeathEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include <Logging.h>
#include <imgui.h>

#include "Registry.h"
#include "Helpers/ImGuiExtras.h"
#include "Helpers/PlayerUtils.h"
#include "Helpers/ActorUtils.h"
#include "Helpers/EntityUtils.h"

#define TAG "[ZActorAttachOnDeathEffect] "

bool ZActorAttachOnDeathEffect::Available() const
{
    return ZActorWellbeingChangeEffectBase::Available() && ZActorUtilsEffectBase::Available();
}

void ZActorAttachOnDeathEffect::OnDrawDebugUI()
{
    ImGuiEx::DragFloat("Attacher Upwards Offset", &m_fUpwardsOffset, 0.0f, 10.0f);
    ImGuiEx::DragFloat("Detach Distance", &m_fDetachDistance, 0.1f, 10.0f);
    ImGui::Checkbox("Detach on Contact", &m_bDetachOnContact);
    ImGui::Checkbox("Parent to Player on Contact", &m_bParentToPlayerOnContact);
}

void ZActorAttachOnDeathEffect::Start()
{
    m_aRagdollBindings.clear();
    m_bActive = true;

    ZActorWellbeingChangeEffectBase::Start();
}

void ZActorAttachOnDeathEffect::Stop()
{
    ZActorWellbeingChangeEffectBase::Stop();

    for (auto& s_RagdollBinding : m_aRagdollBindings)
    {
        s_RagdollBinding.Detach();
    }

    m_bActive = false;
}

void ZActorAttachOnDeathEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    ZActorWellbeingChangeEffectBase::OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);
    ZActorUtilsEffectBase::OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);

    if (!m_bActive)
    {
        return;
    }

    // get up to 999 (so all) nearby actors at ~1m radius
    const auto s_rPlayer = Utils::GetLocalPlayer();
    const auto s_rPlayerHead = Utils::GetPlayerHeadAttachEntity(s_rPlayer);
    const auto s_rPlayerSpatial = TEntityRef<ZSpatialEntity>(s_rPlayer.m_entityRef);
    const auto s_mPlayerTransform = s_rPlayerSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();
    for (auto& [s_rNearbyActor, s_fDistance]: Utils::GetNearbyActors(s_mPlayerTransform.Pos, 999, m_fDetachDistance, true, true))
    {
        if (auto s_RagdollBinding = GetRagdollAttacherBinding(s_rNearbyActor))
        {
            if (std::count(m_aRagdollBindings.begin(), m_aRagdollBindings.end(), s_RagdollBinding) == 0)
            {
                // not attached OR not attached by us, ignore.
                continue;
            }

            // detach nearby ragdolls
            if (m_bDetachOnContact)
            {
                s_RagdollBinding.Detach();
            }
            else if (m_bParentToPlayerOnContact)
            {
                if (auto s_rAttacherSpatial = s_RagdollBinding.QuerySpatial())
                {
                    ZEntityRef s_rAttacherSpatialEntity;
                    s_rAttacherSpatial.m_pInterfaceRef->GetID(s_rAttacherSpatialEntity);

                    Utils::SetProperty<TEntityRef<ZSpatialEntity>>(s_rAttacherSpatialEntity, "m_eidParent", s_rPlayerHead);

                    SMatrix43 s_mLocalTransform = SMatrix43();
                    s_mLocalTransform.Trans.z += 1.f;
                    Utils::SetProperty<SMatrix43>(s_rAttacherSpatialEntity, "m_mTransform", s_mLocalTransform);
                }
            }
        }
    }

}

void ZActorAttachOnDeathEffect::OnActorWellbeingChanged(TEntityRef<ZActor> p_rActor, const SActorState& p_OldState, const SActorState& p_NewState)
{
    // pacified or died?
    if ((!p_OldState.m_bPacified && p_NewState.m_bPacified) || (!p_OldState.m_bDead && p_NewState.m_bDead))
    {
        Logger::Debug(TAG "Actor '{}' died or was pacified", p_rActor.m_pInterfaceRef->m_sActorName);
        OnActorPerished(p_rActor);
        return;
    }
}

void ZActorAttachOnDeathEffect::OnActorPerished(TEntityRef<ZActor> p_rActor)
{
    if (const auto s_rActorSpatial = TEntityRef<ZSpatialEntity>(p_rActor.m_entityRef))
    {
        auto s_mActorTransform = s_rActorSpatial.m_pInterfaceRef->GetObjectToWorldMatrix();

        // move the transform up to head height
        s_mActorTransform.Pos += s_mActorTransform.Up * m_fUpwardsOffset;

        // attach the ragdoll at head height
        auto s_RagdollAttachBinding = GetRagdollAttacherBinding(p_rActor);
        if (auto s_rAttacherSpatial = s_RagdollAttachBinding.QuerySpatial())
        {
            s_rAttacherSpatial.m_pInterfaceRef->SetObjectToWorldMatrixFromEditor(s_mActorTransform);

            s_RagdollAttachBinding.Attach();

            m_aRagdollBindings.push_back(s_RagdollAttachBinding);
        }
    }
}

// thb kinda disturbing effect
//REGISTER_CHAOS_EFFECT(ZActorAttachOnDeathEffect);
