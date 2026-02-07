#include "CameraUtils.h"

#include "Globals.h"
#include "Functions.h"
#include "Logging.h"

#include "EntityUtils.h"

#define TAG "[CameraUtils] "

bool Utils::GetActiveCamera(ZEntityRef &p_ActiveCameraEntity)
{
    TEntityRef<IRenderDestinationEntity> s_RenderDestination;
    if (!GetRenderDestinationEntity(s_RenderDestination))
    {
        return false;
    }

    auto s_pActiveCameraEntity = s_RenderDestination.m_pInterfaceRef->GetSource();
    if (!s_pActiveCameraEntity)
    {
        return false;
    }

    p_ActiveCameraEntity = *s_pActiveCameraEntity;
    return true;
}

bool Utils::SetActiveCamera(ZEntityRef &p_NewCameraEntity)
{
    if (!p_NewCameraEntity)
    {
        return false;
    }

    TEntityRef<IRenderDestinationEntity> s_RenderDestination;
    if (!GetRenderDestinationEntity(s_RenderDestination))
    {
        return false;
    }

    s_RenderDestination.m_pInterfaceRef->SetSource(&p_NewCameraEntity);
    return true;
}

bool Utils::IsMainHitmanCamera(ZEntityRef &p_CameraEntity)
{
    constexpr uint64_t c_nMainHitmanCameraEntityId = 1520968122659560567;
    constexpr uint64_t c_nMainHitmanCameraOwningEntityId = 5884861939421351664;

    uint64_t s_nCameraEntityId = 0,
             s_nOwningEntityId = 0;

    // TODO: holy f this is ugly
    if (const auto s_pCameraEntity = p_CameraEntity.GetEntity())
    {
        if (const auto s_pCameraEntityType = s_pCameraEntity->GetType())
        {
            s_nCameraEntityId = s_pCameraEntityType->m_nEntityID;
        }

        if (const auto s_OwningEntity = p_CameraEntity.GetOwningEntity())
        {
            if (const auto s_pOwningEntity = s_OwningEntity.GetEntity())
            {
                if (const auto s_pOwningEntityType = s_pOwningEntity->GetType())
                {
                    s_nOwningEntityId = s_pOwningEntityType->m_nEntityID;
                }
            }
        }
    }

    // check by entity IDs
    if (s_nCameraEntityId != c_nMainHitmanCameraEntityId ||
        s_nOwningEntityId != c_nMainHitmanCameraOwningEntityId)
    {
        return false;
    }

    // validate type name
    return Utils::GetEntityTypeName(p_CameraEntity) == "ZHM5MainCamera";
}

bool Utils::GetRenderDestinationEntity(TEntityRef<IRenderDestinationEntity> &p_RenderDestinationEntity)
{
    Functions::ZCameraManager_GetActiveRenderDestinationEntity->Call(
        Globals::CameraManager,
        &p_RenderDestinationEntity);

    if (!p_RenderDestinationEntity)
    {
        Logger::Debug(TAG "No active render destination entity.");
        return false;
    }

    return true;
}
