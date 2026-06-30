#include "YVRHandMgr.h"
#include "../../YVR_Lib_Types.h"
#include "../../WriteLog.h"
#include <string>


YVRHandMgr::YVRHandMgr() {
	initHandTracking();
}

void YVRHandMgr::initHandTracking()
{

	// L
	scaleL.next = nullptr;
	capsuleStateL.next = &scaleL;
	aimStateL.next = &capsuleStateL;
	velocitiesL.next = &aimStateL;
	velocitiesL.jointCount = XR_HAND_JOINT_COUNT_EXT;
	velocitiesL.jointVelocities = jointVelocityL;
	locationsL.next = &velocitiesL;
	locationsL.jointCount = XR_HAND_JOINT_COUNT_EXT;
	locationsL.jointLocations = jointLocationsL;

	/// R
	scaleR.next = nullptr;
	scaleR.overrideHandScale = XR_FALSE;  // XR_TRUE;
	capsuleStateR.next = &scaleR;
	aimStateR.next = &capsuleStateR;
	velocitiesR.next = &aimStateR;
	velocitiesR.jointCount = XR_HAND_JOINT_COUNT_EXT;
	velocitiesR.jointVelocities = jointVelocityR;
	locationsR.next = &velocitiesR;
	locationsR.jointCount = XR_HAND_JOINT_COUNT_EXT;
	locationsR.jointLocations = jointLocationsR;
	isInit = true;
}

void YVRHandMgr::SetHandJontLocations(yvrHMDState* yvrHMD)
{
	memcpy(&locationsL, &yvrHMD->leftHandControllers, sizeof(XrHandJointLocationsEXT));
	memcpy(&jointLocationsL, &yvrHMD->leftJointLocations, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&velocitiesL, &yvrHMD->velocitiesL, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&jointVelocityL, &yvrHMD->jointVelocitiesL, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&aimStateL, &yvrHMD->aimStateL, sizeof(XrHandTrackingAimStateFB));
	memcpy(&capsuleStateL, &yvrHMD->capsuleStateL, sizeof(XrHandTrackingCapsulesStateFB));
	memcpy(&scaleL, &yvrHMD->scaleL, sizeof(XrHandTrackingScaleFB));

	memcpy(&locationsR, &yvrHMD->rightHandControllers, sizeof(XrHandJointLocationsEXT));
	memcpy(&jointLocationsR, &yvrHMD->rightJointLocations, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&velocitiesR, &yvrHMD->velocitiesR, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&jointVelocityR, &yvrHMD->jointVelocitiesR, sizeof(XrHandJointLocationEXT) * XR_HAND_JOINT_COUNT_EXT);
	memcpy(&aimStateR, &yvrHMD->aimStateR, sizeof(XrHandTrackingAimStateFB));
	memcpy(&capsuleStateL, &yvrHMD->capsuleStateR, sizeof(XrHandTrackingCapsulesStateFB));
	memcpy(&scaleR, &yvrHMD->scaleR, sizeof(XrHandTrackingScaleFB));

	isHandTrackingEnable = locationsL.isActive || locationsR.isActive;
}

void YVRHandMgr::GetHandHandJointLocations(const Handedness handedness, YVRHandJointLocations* handJointLocations,
	XrHandJointLocationEXT* refLocations, XrHandJointVelocityEXT* refVelocities)
{
	switch (handedness)
	{
	case HAND_LEFT:
		setHandJointInfo(scaleL, aimStateL, locationsL, jointLocationsL, jointVelocityL,
			handJointLocations, refLocations, refVelocities);
		break;
	case HAND_RIGHT:
		setHandJointInfo(scaleR, aimStateR, locationsR, jointLocationsR, jointVelocityR,
			handJointLocations, refLocations, refVelocities);
		break;
	default:
		break;
	}
}


XrInputDeviceFlagsYVR YVRHandMgr::GetCurrentInputDevice()
{
	return isHandTrackingEnable ? XR_INPUT_DEVICES_YVR_HAND_BIT_YVR : XR_INPUT_DEVICES_YVR_CONTROLLER_BIT_YVR;
}


void YVRHandMgr::setHandJointInfo(XrHandTrackingScaleFB scale, XrHandTrackingAimStateFB aimState,
	XrHandJointLocationsEXT locations,
	XrHandJointLocationEXT* jointLocations, XrHandJointVelocityEXT* jointVelocities,
	YVRHandJointLocations* handJointLocations,
	XrHandJointLocationEXT* refLocations, XrHandJointVelocityEXT* refVelocities)
{
	for (size_t i = 0; i < locations.jointCount; i++)
	{
		refVelocities[i] = jointVelocities[i];
		refLocations[i] = jointLocations[i];
	}

	handJointLocations->isActive = locations.isActive;
	handJointLocations->jointCount = locations.jointCount;
	handJointLocations->handScale = scale.currentOutput;
	handJointLocations->aimState.aimPose = aimState.aimPose;
	handJointLocations->aimState.status = aimState.status;
	handJointLocations->aimState.pinchStrengthIndex = aimState.pinchStrengthIndex;
	handJointLocations->aimState.pinchStrengthLittle = aimState.pinchStrengthLittle;
	handJointLocations->aimState.pinchStrengthMiddle = aimState.pinchStrengthMiddle;
	handJointLocations->aimState.pinchStrengthRing = aimState.pinchStrengthRing;
}
