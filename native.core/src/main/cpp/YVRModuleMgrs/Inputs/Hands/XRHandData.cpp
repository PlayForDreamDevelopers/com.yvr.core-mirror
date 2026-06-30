#include "XRHandData.h"

XRHandData::XRHandData()
{
    info = {XR_TYPE_HAND_JOINTS_LOCATE_INFO_EXT};
    scale = {XR_TYPE_HAND_TRACKING_SCALE_FB, nullptr, 0, 0, true, 0};
    capsuleState = {XR_TYPE_HAND_TRACKING_CAPSULES_STATE_FB, &scale, {}};
    aimState = {XR_TYPE_HAND_TRACKING_AIM_STATE_FB, &capsuleState, 0, {}, 0, 0, 0, 0};
    velocities = {XR_TYPE_HAND_JOINT_VELOCITIES_EXT, &aimState, XR_HAND_JOINT_COUNT_EXT, velocitiesData};
    locations = {XR_TYPE_HAND_JOINT_LOCATIONS_EXT, &velocities, false, XR_HAND_JOINT_COUNT_EXT, locationsData};
}
