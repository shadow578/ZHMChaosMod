#pragma once
#include <Glacier/ZEntity.h>
#include <Glacier/ZCameraEntity.h>

namespace Utils
{
    bool GetActiveCamera(ZEntityRef& p_ActiveCameraEntity);
    bool SetActiveCamera(ZEntityRef& p_NewCameraEntity);
    bool IsMainHitmanCamera(ZEntityRef& p_CameraEntity);
    bool GetRenderDestinationEntity(TEntityRef<IRenderDestinationEntity>& p_RenderDestinationEntity);
};
