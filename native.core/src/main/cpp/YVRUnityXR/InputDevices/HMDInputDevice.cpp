#include "HMDInputDevice.h"
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"

#define SET_HMD_VEC3(inputInterface, state, usage, data) \
    inputInterface->DeviceState_SetAxis3DValue(state, usage2IndexMap[usage], OpenXRVector2UnityVector(data));

#define SET_HMD_ROTATION(inputInterface, state, usage, data) \
    inputInterface->DeviceState_SetRotationValue(state, usage2IndexMap[usage], OpenXRQuaternion2UnityVector(data));

#define SET_HMD_BINARY(inputInterface, state, usage, data) \
    inputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[usage], data);

#define SET_HMD_State(inputInterface, state, usage, data) \
    inputInterface->DeviceState_SetDiscreteStateValue(state, usage2IndexMap[usage], data);

HMDInputDevice::HMDInputDevice()
{
    requiredUsages = {
            {kUnityXRInputFeatureUsageCenterEyePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageCenterEyeRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageCenterEyeVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageCenterEyeAngularVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageCenterEyeAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageCenterEyeAngularAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageLeftEyePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageLeftEyeRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageRightEyePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageRightEyeRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageDevicePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageDeviceVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAngularVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAngularAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageUserPresence, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageIsTracked, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageTrackingState, kUnityXRInputFeatureTypeDiscreteStates},
        };

}

void HMDInputDevice::fillDeviceDefinition(UnityXRInputDeviceDefinition* definition)
{
    unityXRInputInterface->DeviceDefinition_SetName(definition, "Headset");
    unityXRInputInterface->DeviceDefinition_SetManufacturer(definition, "YVR");
    unityXRInputInterface->DeviceDefinition_SetSerialNumber(definition, "12345");
    unityXRInputInterface->DeviceDefinition_SetCharacteristics(
        definition, static_cast<UnityXRInputDeviceCharacteristics>(
            kUnityXRInputDeviceCharacteristicsHeadMounted | kUnityXRInputDeviceCharacteristicsTrackedDevice));

    for (const auto& pair : requiredUsages)
    {
        usage2IndexMap[pair.first] = unityXRInputInterface->DeviceDefinition_AddFeatureWithUsage(
            definition, pair.first, pair.second, pair.first);
    }
}

void HMDInputDevice::updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType)
{
    const bool isDynamic = updateType == kUnityXRInputUpdateTypeDynamic;
    const XRMotionData* hmdPose = isDynamic ? &plugin.hmdMgr->hmdPhysicsPose : &plugin.hmdMgr->hmdRenderPose;
    const XrPosef* leftEyePose = isDynamic ? &plugin.hmdMgr->lEyePhysicsPose : &plugin.hmdMgr->lEyeRenderPose;
    const XrPosef* rightEyePose = isDynamic ? &plugin.hmdMgr->rEyePhysicsPose : &plugin.hmdMgr->rEyeRenderPose;

    updatePoseData(state, hmdPose, leftEyePose, rightEyePose);
}

void HMDInputDevice::updatePoseData(UnityXRInputDeviceState* state, const XRMotionData* hmdPose, const XrPosef* leftEyePose,
                                    const XrPosef* rightEyePose)
{
    const bool isHeadsetPositionTracked = hmdPose->location.locationFlags & XR_SPACE_LOCATION_POSITION_TRACKED_BIT;
    const bool isHeadsetOrientationTracked = hmdPose->location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;

    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyePosition, hmdPose->location.pose.position);
    SET_HMD_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyeRotation, hmdPose->location.pose.orientation);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyeVelocity, hmdPose->velocity.linearVelocity);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyeAngularVelocity, hmdPose->velocity.angularVelocity);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyeAcceleration, hmdPose->acceleration.linearAcceleration);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageCenterEyeAngularAcceleration, hmdPose->acceleration.angularAcceleration);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageLeftEyePosition, leftEyePose->position);
    SET_HMD_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageLeftEyeRotation, leftEyePose->orientation);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageRightEyePosition, rightEyePose->position);
    SET_HMD_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageRightEyeRotation, rightEyePose->orientation);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDevicePosition, hmdPose->location.pose.position);
    SET_HMD_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceRotation, hmdPose->location.pose.orientation);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceVelocity, hmdPose->velocity.linearVelocity);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAngularVelocity, hmdPose->velocity.angularVelocity);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAcceleration, hmdPose->acceleration.linearAcceleration);
    SET_HMD_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAngularAcceleration, hmdPose->acceleration.angularAcceleration);
    SET_HMD_BINARY(unityXRInputInterface, state, kUnityXRInputFeatureUsageUserPresence, plugin.hmdMgr->isUserPresent());
    SET_HMD_BINARY(unityXRInputInterface, state, kUnityXRInputFeatureUsageIsTracked, isHeadsetPositionTracked | isHeadsetOrientationTracked);

    int trackingState = kUnityXRInputTrackingStateNone;
    if (isHeadsetPositionTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStatePosition);
    if (isHeadsetOrientationTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStateRotation);
    SET_HMD_State(unityXRInputInterface, state, kUnityXRInputFeatureUsageTrackingState, trackingState);
}
