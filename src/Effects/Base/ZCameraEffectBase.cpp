#include "ZCameraEffectBase.h"

#include <Globals.h>
#include <Glacier/ZModule.h>
#include <Glacier/ZScene.h>
#include <Glacier/SExternalReferences.h>

#include "Logging.h"

#include "Helpers/Utils.h"
#include "Helpers/CameraUtils.h"

#define TAG "[ZCameraEffectBase] "

static ZTemplateEntitySpawner<"[assembly:/templates/core/hm5camera.template?/compositeentity_norenderdestination.entitytemplate].pc_entitytype"> g_CameraEntityProp;

void ZCameraEffectBase::LoadResources()
{
	m_pCameraSpawner = g_CameraEntityProp.CreateSession();
}

bool ZCameraEffectBase::Available() const
{
    return IChaosEffect::Available() &&
        m_pCameraSpawner &&
        m_pCameraSpawner->IsAvailable();
}

void ZCameraEffectBase::Start()
{
    if (!EnsureCameraEntity())
    {
        Logger::Debug(TAG "Could not spawn camera entity.");
        m_bIsAvailable = false;
        return;
    }

    if (!Utils::GetActiveCamera(m_OriginalCameraEntity))
    {
        Logger::Error(TAG "Could not get active camera entity.");
        return;
    }

    if (!Utils::IsMainHitmanCamera(m_OriginalCameraEntity))
    {
        Logger::Error(TAG "Active camera entity is not the main Hitman camera, aborting");
        return;
    }

    m_bEffectCameraActive = Utils::SetActiveCamera(m_EffectCameraEntity);
}

void ZCameraEffectBase::Stop()
{
    if (!m_OriginalCameraEntity)
    {
        return;
    }

    if (!Utils::SetActiveCamera(m_OriginalCameraEntity))
    {
        Logger::Error(TAG "Could not restore original camera entity.");
    }

    m_bEffectCameraActive = false;
    m_OriginalCameraEntity = {};
}

void ZCameraEffectBase::OnClearScene()
{
    m_bEffectCameraActive = false;
    m_EffectCameraEntity = {};
    m_OriginalCameraEntity = {};
	m_pCameraSpawner = nullptr;
}

void ZCameraEffectBase::OnDrawDebugUI()
{
    ImGui::TextUnformatted(fmt::format("Camera Prop: {}", m_pCameraSpawner->ToString()).c_str());

    ImGui::TextUnformatted(fmt::format("Effect Camera Active: {}", m_bEffectCameraActive ? "Yes" : "No").c_str());
    ImGui::TextUnformatted(fmt::format("Effect Camera Entity: {}", m_EffectCameraEntity ? "Valid" : "Invalid").c_str());
    ImGui::TextUnformatted(fmt::format("Original Camera Entity: {}", m_OriginalCameraEntity ? "Valid" : "Invalid").c_str());
}

bool ZCameraEffectBase::IsCompatibleWith(const IChaosEffect* p_pOther) const
{
    return IChaosEffect::IsCompatibleWith(p_pOther) 
        // all camera effects are incompatible with each other
        && !Utils::IsInstanceOf<ZCameraEffectBase>(p_pOther);
}

bool ZCameraEffectBase::EnsureCameraEntity()
{
    // only once
    if (m_EffectCameraEntity)
    {
        return true;
    }

    const auto s_Scene = Globals::Hitman5Module->m_pEntitySceneContext->m_pScene;
    if (!s_Scene)
    {
        Logger::Debug(TAG "Scene not loaded.");
        return false;
    }

    if (!m_pCameraSpawner)
    {
        Logger::Debug(TAG "Camera spawner session not initialized.");
        return false;
    }

	auto m_CameraHolderEntity = m_pCameraSpawner->Spawn();
    if (!m_CameraHolderEntity)
    {
        Logger::Debug(TAG "Could not spawn camera entity.");
        return false;
    }

    // inside the template, grab camera entity "HM5MainCamera01"
    auto s_CameraBpFactory = m_CameraHolderEntity.GetBlueprintFactory();
    if (const auto idx = s_CameraBpFactory->GetSubEntityIndex(0x151b9072f9d0dc77); idx != -1)
    {
        if (const auto s_Ent = s_CameraBpFactory->GetSubEntity(m_CameraHolderEntity.m_pEntity, idx))
        {
            m_EffectCameraEntity = s_Ent;
        }
    }

    // ensure the overhead camera entity implements
    auto s_CameraInterfaceRef = m_EffectCameraEntity.QueryInterface<ICameraEntity>();
    if (!s_CameraInterfaceRef)
    {
        Logger::Debug(TAG "Spawned camera entity does not implement ICameraEntity.");
        return false;
    }

    return true;
}
