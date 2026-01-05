#pragma once
#include <Glacier/ZEntity.h>
#include <Glacier/ZCameraEntity.h>

namespace Utils
{
    /**
     * Get the currently active camera entity.
     */
    bool GetActiveCamera(ZEntityRef& p_ActiveCameraEntity);

    /**
     * Set the entity as the active camera.
     */
    bool SetActiveCamera(ZEntityRef& p_NewCameraEntity);

    /**
     * Check if a entity is the main, default game camera entity.
     */
    bool IsMainHitmanCamera(ZEntityRef& p_CameraEntity);

    /**
     * Get the IRenderDestinationEntity that is currently active for the main screen.
     */
    bool GetRenderDestinationEntity(TEntityRef<IRenderDestinationEntity>& p_RenderDestinationEntity);
};
