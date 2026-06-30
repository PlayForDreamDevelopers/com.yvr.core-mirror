#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include "openxr/openxr.h"
#include "../../YVR_Lib_Types.h"

#define HAND_POSE_LENGTH 2000

// Flag bits for XrInputDeviceFlagsYVR
typedef XrFlags64 XrInputDeviceFlagsYVR;
static const XrInputDeviceFlagsYVR XR_INPUT_DEVICES_YVR_CONTROLLER_BIT_YVR = 0x00000001;
static const XrInputDeviceFlagsYVR XR_INPUT_DEVICES_YVR_HAND_BIT_YVR = 0x00000002;

/// Specifies left or right handedness.
typedef enum Handedness_
{
	HAND_UNKNOWN = 0,
	HAND_LEFT = 1,
	HAND_RIGHT = 2
} Handedness;

// XrHandTrackingAimStateFB extends XrHandJointLocationsEXT
typedef struct YVRHandTrackingAimState_
{
	XrHandTrackingAimFlagsFB status;
	XrPosef aimPose;
	float pinchStrengthIndex;
	float pinchStrengthMiddle;
	float pinchStrengthRing;
	float pinchStrengthLittle;
} YVRHandTrackingAimState;

typedef struct YVRHandJointLocations_
{
	uint32_t isActive;
	uint32_t jointCount;
	float handScale;
	YVRHandTrackingAimState aimState{};
} YVRHandJointLocations;


class YVRHandMgr
{
public:
	bool isHandTrackingEnable = false;
	bool isInit = false;

	YVRHandMgr();
	~YVRHandMgr();

	void SetHandJontLocations(yvrHMDState* yvrHMD);
	void GetHandHandJointLocations(const Handedness handedness, YVRHandJointLocations* handJointLocations,
		XrHandJointLocationEXT* refLocations, XrHandJointVelocityEXT* refVelocities);
	XrInputDeviceFlagsYVR GetCurrentInputDevice();

private:
	/// Hands - data buffers
	XrHandJointLocationsEXT locationsL{ XR_TYPE_HAND_JOINT_LOCATIONS_EXT };
	XrHandJointLocationEXT jointLocationsL[XR_HAND_JOINT_COUNT_EXT];
	XrHandJointVelocitiesEXT velocitiesL{ XR_TYPE_HAND_JOINT_VELOCITIES_EXT };
	XrHandJointVelocityEXT jointVelocityL[XR_HAND_JOINT_COUNT_EXT];
	XrHandTrackingAimStateFB aimStateL{ XR_TYPE_HAND_TRACKING_AIM_STATE_FB };
	XrHandTrackingCapsulesStateFB capsuleStateR{ XR_TYPE_HAND_TRACKING_CAPSULES_STATE_FB };
	XrHandTrackingScaleFB scaleL{ XR_TYPE_HAND_TRACKING_SCALE_FB };

	XrHandJointLocationsEXT locationsR{ XR_TYPE_HAND_JOINT_LOCATIONS_EXT };
	XrHandJointLocationEXT jointLocationsR[XR_HAND_JOINT_COUNT_EXT];
	XrHandJointVelocitiesEXT velocitiesR{ XR_TYPE_HAND_JOINT_VELOCITIES_EXT };
	XrHandJointVelocityEXT jointVelocityR[XR_HAND_JOINT_COUNT_EXT];
	XrHandTrackingAimStateFB aimStateR{ XR_TYPE_HAND_TRACKING_AIM_STATE_FB };
	XrHandTrackingCapsulesStateFB capsuleStateL{ XR_TYPE_HAND_TRACKING_CAPSULES_STATE_FB };
	XrHandTrackingScaleFB scaleR{ XR_TYPE_HAND_TRACKING_SCALE_FB };


	void initHandTracking();

	void setHandJointInfo(XrHandTrackingScaleFB scale, XrHandTrackingAimStateFB aimState,
		XrHandJointLocationsEXT locations,
		XrHandJointLocationEXT* jointLocations, XrHandJointVelocityEXT* jointVelocities,
		YVRHandJointLocations* handJointLocations,
		XrHandJointLocationEXT* refLocations, XrHandJointVelocityEXT* refVelocities);
};
