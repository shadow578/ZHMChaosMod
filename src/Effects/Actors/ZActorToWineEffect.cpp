#include "ZActorToWineEffect.h"

#include <Glacier/ZEntity.h>
#include <Glacier/ZActor.h>
#include <Glacier/ZSpatialEntity.h>

#include "Logging.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"

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

void ZActorToWineEffect::OnDrawDebugUI()
{
    ImGui::Checkbox("Teleport Bodies", &m_bTeleportBodies);
}

void ZActorToWineEffect::OnActorWellbeingChanged(ZActor* p_pActor, const SActorState& p_OldState, const SActorState& p_NewState)
{
    if (!p_OldState.m_bDead && p_NewState.m_bDead && !p_pActor->m_bBodyHidden)
    {
        Logger::Debug(TAG "Actor '{}' died", p_pActor->m_sActorName);
        OnActorPerished(p_pActor);
        return;
    }
}

void ZActorToWineEffect::OnActorPerished(ZActor* p_pActor)
{
	ZEntityRef s_rActor;
	p_pActor->GetID(s_rActor);
    if (!s_rActor)
    {
        return;
    }

    if (auto* s_pActorSpatial = s_rActor.QueryInterface<ZSpatialEntity>())
    {
        auto s_mActorTransform = s_pActorSpatial->GetObjectToWorldMatrix();

        // spawn wine bottle slightly above the actor
        auto s_vPos = s_mActorTransform.Trans;
        s_vPos += (s_mActorTransform.Up.Normalized()) * 0.1f;

        SpawnRepositoryPropAt(m_WineBottleProp, s_vPos);

        // mark body hidden
        // idk if this works...
        p_pActor->m_bBodyHidden = true;

        // make the actor invisible
        s_rActor.SetProperty("m_bVisible", false);

        if (!m_bTeleportBodies)
        {
            return;
        }

        // aquire ref to physics entity
        ZEntityRef s_rPhysicsSystem;
        if (auto* s_pBlueprint = Utils::GetEntityBlueprintFactoryFor(s_rActor))
        {
            // [assembly:/templates/gameplay/ai2/actors.template?/npcactor.entitytemplate].pc_entitytype
            // sub-entity "PhysicsSystem" of "NPCActor"
            if (const auto s_nIdx = s_pBlueprint->GetSubEntityIndex(0x66aaaad61b6a51a5); s_nIdx != -1)
            {
                if (auto* s_pEntity = s_pBlueprint->GetSubEntity(s_rActor.m_pObj, s_nIdx))
                {
                    s_rPhysicsSystem = ZEntityRef(s_pEntity);
                }
            }
        }

        // disable physics of ragdoll so the TP sticks
        if (s_rPhysicsSystem)
        {
            s_rPhysicsSystem.SetProperty("m_bActive", false);
        }
        else
        {
            Logger::Debug(TAG "could not get PhysicsSystem sub-entity of NPCActor {}", p_pActor->GetActorName());
        }

        // setting the body invisible does NOT prevent interactions, so teleport out of the way
        s_mActorTransform.Pos.x = 0.f;
		s_mActorTransform.Pos.y = 0.f;
		s_mActorTransform.Pos.z = -999.f;
        s_pActorSpatial->SetObjectToWorldMatrixFromEditor(s_mActorTransform);
    }
}

REGISTER_CHAOS_EFFECT(ZActorToWineEffect);
