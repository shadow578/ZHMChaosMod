#include "ZFixCameraUnlocker.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/CameraUtils.h"

void ZFixCameraUnlocker::Start()
{
    auto s_aCameras = Utils::ZEntityFinder()
        .EntityType("ZHM5MainCamera")
        .Find();
    if (s_aCameras.empty())
    {
        return;
    }

    for (auto& s_Camera : s_aCameras)
    {
        if (Utils::IsMainHitmanCamera(s_Camera))
        {
            Utils::SetActiveCamera(s_Camera);
        }
    }
}

REGISTER_CHAOS_UNLOCKER(ZFixCameraUnlocker)
