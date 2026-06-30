#pragma once

#include "openxr/openxr.h"

class XRHandData
{
public:
    XRHandData();
    XrHandJointsLocateInfoEXT info;
    XrHandTrackingScaleFB scale;
    XrHandTrackingCapsulesStateFB capsuleState;
    XrHandTrackingAimStateFB aimState;
    XrHandJointVelocitiesEXT velocities;
    XrHandJointLocationsEXT locations;

private:
    XrHandJointVelocityEXT velocitiesData[XR_HAND_JOINT_COUNT_EXT];
    XrHandJointLocationEXT locationsData[XR_HAND_JOINT_COUNT_EXT];
};
