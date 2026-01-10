#pragma once
#include "Helpers/EntityBinding.h"

#include <Glacier/Enums.h>

class ZValueBool;

// [modules:/zhm5singledoor2.class].pc_entitytype
// compatible with [modules:/zhm5doubledoor2.class].pc_entitytype 
struct SHM5SingleDoor2Binding
{
	BINDING_CONSTRUCTOR(SHM5SingleDoor2Binding);

	PROPERTY_RO(IHM5Door_EInitialState, m_InitialState); // initial state of the door on scene load
	PROPERTY(IHM5Door_EOpenDir, m_OpenDir); // what direction the door opens when activated
	PROPERTY(float32, m_fRotAngle); // rotation angle when door is opened
	PROPERTY(TEntityRef<ZValueBool>, m_rAutoClose); // auto close enabled?
	PROPERTY(TEntityRef<ZValueBool>, m_rManualClose); // manual close allowed?
	PROPERTY(float32, m_fCloseTime); // how many seconds to auto-close. (seems to reset on auto-close!)
	PROPERTY(IHM5Door_EOpenMode, m_OpenMode);
	PROPERTY(bool, m_bNPCUseEnable); // doesn't work?
	PROPERTY(bool, m_bNeverCloseGate);
	PROPERTY(bool, m_bIgnoreCovers);
	PROPERTY(float32, m_fAISoundModifierValue);

	INPUT_PIN(LockDoor); // Lock door
	INPUT_PIN(UnlockDoor); // Unlock door
	INPUT_PIN(OpenDoorIn); // Open door inwards
	INPUT_PIN(OpenDoorOut); // Open door outwards
	INPUT_PIN(CloseDoor); // Close door
	INPUT_PIN(BurstOpen); // Burst up door inwards, will set the door as broken
	INPUT_PIN(BurstOpenIn); // Burst up door inwards, will set the door as broken
	INPUT_PIN(BurstOpenOut); // Burst up door outwards, will set the door as broken
	INPUT_PIN(DoorBroken); // Door is broken by damage or simula
	INPUT_PIN(MakeVisible);
	INPUT_PIN(MakeInvisible);
	INPUT_PIN(SetVisible);
	INPUT_PIN(Detach);

	OUTPUT_PIN(DoorOpen); // Fired when door is opened by Hitman.
};
