#pragma once
#include "Helpers/EntityBinding.h"

#include <Glacier/Enums.h>

// [assembly:/_pro/design/templates/camera/pictureinpicture.template?/pip_camera_custom_event.entitytemplate].pc_entitytype
struct SPIPCameraCustomEventBindingBinding {
	BINDING_CONSTRUCTOR(SPIPCameraCustomEventBindingBinding);

	// ZSpatialEntity
	PROPERTY(TEntityRef<ZSpatialEntity>, m_eidParent);	// Camera spatial parent
	PROPERTY(SMatrix43, m_mTransform);					// Camera spatial local transform

	PROPERTY(bool, m_bValueusepiptimer);				// automatically close PIP?
	PROPERTY(int32, m_nValuePiPPriority);				// priority over other PIP cameras (higher overrides lower)
	PROPERTY(bool, m_bAutoAspect);						// automatically set aspect ratio?
	PROPERTY(float32, m_fAspectWByH);					// manual aspect ratio (width / height), only used if m_bAutoAspect is false
	PROPERTY(float32, m_fFovYDeg);						// manual FOV in degrees
	PROPERTY(ZRuntimeResourceID, m_rHUDMessagepip);		// HUD message shown below the camera view. if not set, only the camera feed is shown without the message box
	PROPERTY(ZPIPMessageEntity_EIcon, m_eIcon);			// icon shown in the HUD message. only used if m_rHUDMessagepip is set

	PROPERTY(bool, m_bAllowAutoCameraCuts);
	PROPERTY(float32, m_fNearZ);
	PROPERTY(float32, m_fFarZ);
	PROPERTY(bool, m_bIsUICamera);
	PROPERTY(bool, ShowOnlyOnce);
	PROPERTY(bool, m_bForceVisible);
	PROPERTY(bool, PlayOncePRDuration);
	PROPERTY(bool, PiP_Sound_Enabled);
	PROPERTY(int32, int_PiPDuriation); // ms

	INPUT_PIN(Activate);	// Activate & show PIP
	INPUT_PIN(Deactivate);	// Hide PIP
};