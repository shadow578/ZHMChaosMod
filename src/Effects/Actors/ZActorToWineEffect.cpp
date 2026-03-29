#include "ZActorToWineEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include <Logging.h>
#include <imgui.h>

#include "Registry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/ActorUtils.h"
#include "Entity/EntityIds.h"

#define TAG "[ZActorToWineEffect] "

void ZActorToWineEffect::LoadResources()
{
    ZActorWellbeingChangeEffectBase::LoadResources();
    ZSpawnRepositoryItemEffectBase::LoadResources();

    // pre-load wine bottle repository prop for later use
    // Melee_Llama_WineBottle_WhiteLabel_Vintage / "1945 Grand Paladin"
    m_WineBottleProp = GetRepositoryPropByID("2d960bf0-217c-400d-a1ee-f721e18f2926");
    if (!m_WineBottleProp)
    {
        Logger::Error(TAG "Failed to load wine bottle repository prop");
        m_bIsAvailable = false;
    }
}

void ZActorToWineEffect::OnClearScene()
{
    ZActorWellbeingChangeEffectBase::OnClearScene();
    ZSpawnRepositoryItemEffectBase::OnClearScene();
    m_WineBottleProp = {};
}

void ZActorToWineEffect::OnDrawDebugUI()
{
    ImGui::Checkbox("Teleport Bodies", &m_bTeleportBodies);
}

bool ZActorToWineEffect::Available() const
{
    return ZActorWellbeingChangeEffectBase::Available()
           && ZSpawnRepositoryItemEffectBase::Available()
           && m_WineBottleProp;
}

void ZActorToWineEffect::Start()
{
    ZActorWellbeingChangeEffectBase::Start();
}

void ZActorToWineEffect::Stop()
{
    ZActorWellbeingChangeEffectBase::Stop();
}

void ZActorToWineEffect::OnSlowUpdate(const float32 p_fDeltaTime, const float32 p_fEffectTimeRemaining)
{
    ZActorWellbeingChangeEffectBase::OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);
    ZSpawnRepositoryItemEffectBase::OnSlowUpdate(p_fDeltaTime, p_fEffectTimeRemaining);
}

void ZActorToWineEffect::OnActorWellbeingChanged(TEntityRef<ZActor> p_rActor, const SActorState& p_OldState, const SActorState& p_NewState)
{
    if (!p_OldState.m_bDead && p_NewState.m_bDead && !p_rActor.m_pInterfaceRef->m_bBodyHidden)
    {
        Logger::Debug(TAG "Actor '{}' died", p_rActor.m_pInterfaceRef->m_sActorName);
        OnActorPerished(p_rActor);
        return;
    }
}

void ZActorToWineEffect::OnActorPerished(TEntityRef<ZActor> p_rActor)
{
    if (auto* s_pActorSpatial = p_rActor.m_entityRef.QueryInterface<ZSpatialEntity>())
    {
        auto s_mActorTransform = s_pActorSpatial->GetObjectToWorldMatrix();

        // spawn wine bottle slightly above the actor
        auto s_vPos = s_mActorTransform.Trans;
        s_vPos += (s_mActorTransform.Up.Normalized()) * 0.1f;

        SpawnRepositoryPropAt(m_WineBottleProp, s_vPos);

        // mark body hidden
        // idk if this works...
        p_rActor.m_pInterfaceRef->m_bBodyHidden = true;

        // make the actor invisible
        p_rActor.m_entityRef.SetProperty("m_bVisible", false);

        if (!m_bTeleportBodies)
        {
            return;
        }

        // disable physics of ragdoll so the TP sticks
        Utils::SetRagdollPhysicsEnabled(p_rActor, false);

        // setting the body invisible does NOT prevent interactions, so teleport out of the way
        s_mActorTransform.Pos.x = 0.f;
        s_mActorTransform.Pos.y = 0.f;
        s_mActorTransform.Pos.z = -999.f;
        s_pActorSpatial->SetObjectToWorldMatrixFromEditor(s_mActorTransform);
    }
}

REGISTER_CHAOS_EFFECT(ZActorToWineEffect);
