#pragma once
#include "Entity/Bindings/EntityBinding.h"

#include <Glacier/ZMath.h>

// [assembly:/_pro/scenes/bricks/gameessentialsbase.brick].pc_entitytype
// entity "ChaosMod_HUD_IV" added by companion mod version 1.4.0 via patch cm_hud_image_and_video_view.entity.patch.json
// NOTE: position and size are shared by both image and video, but behave diffently. use debug tool to ensure proper positioning.
struct SImageVideoViewEntityBinding : public SEntityBinding
{
    using SEntityBinding::SEntityBinding;

    // position of the view inside the (full-screen) container. Z may be set, but is recommended not to.
    // for image: position of the center; x from -960~960, y from -540~540
    // for video: position of bottom right corner; x from -960~960, y from -540~540
    PROPERTY(SVector3, m_vPositionOffset);

    // size of the view.
    // for image: % scale of the original image, from 0~1.
    // for video: tbh, no idea about units, just test via debug tool.
    PROPERTY(SVector2, m_vSize);

    PROPERTY(SVector3, m_vRotation);          // rotation of the views.
    PROPERTY(bool, m_bImageIsVisible);        // is the image view visible? note that image and video overlap.
    PROPERTY(bool, m_bVideoIsVisible);        // is the video view visible? note that image and video overlap.
    PROPERTY(ZRuntimeResourceID, m_ridImage); // resource id displayed in the image view. must be loaded beforehand. supports .pc_gfx
    PROPERTY(ZRuntimeResourceID, m_ridVideo); // resource id of the video. must be loaded beforehand. supports .pc_gfxv
    PROPERTY(bool, m_bVideoLoop);             // should the video loop?

    INPUT_PIN(PlayVideo); // start playing the video. before playback starts, the video is not shown.
    INPUT_PIN(StopVideo); // stop playing the video. the video will be invisible after this, until PlayVideo is called again.
};
