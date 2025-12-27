#include "ZFixCameraUnlocker.h"

#include "EffectRegistry.h"
#include "Helpers/EntityUtils.h"
#include "Helpers/CameraUtils.h"

void ZFixCameraUnlocker::Start()
{
    const Utils::EntityFinder::SSearchParams s_Query{
           .m_sEntityType = "ZHM5MainCamera"
    };
    auto s_aCameras = Utils::EntityFinder::FindEntities(s_Query);
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
