#include "EyeTrackingInputDevice.h"
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"

#define SET_EYES_VALUE(inputInterface, state, usage, data) inputInterface->DeviceState_SetEyesValue(state, usage2IndexMap[usage], data);
#define SET_EYES_State(inputInterface, state, usage, data) inputInterface->DeviceState_SetDiscreteStateValue(state, usage2IndexMap[usage], data);
#define SET_EYES_BINARY(inputInterface, state, usage, data) inputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[usage], data);
#define SET_EYE_VEC3(inputInterface, state, usage, data) inputInterface->DeviceState_SetAxis3DValue(state, usage2IndexMap[usage], OpenXRVector2UnityVector(data));
#define SET_EYE_ROTATION(inputInterface, state, usage, data) inputInterface->DeviceState_SetRotationValue(state, usage2IndexMap[usage], OpenXRQuaternion2UnityVector(data));

EyeTrackingInputDevice::EyeTrackingInputDevice()
{
    requiredUsages = {
            {kUnityXRInputFeatureUsageEyesData, kUnityXRInputFeatureTypeEyes},
            {kUnityXRInputFeatureUsageDevicePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageIsTracked, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageTrackingState, kUnityXRInputFeatureTypeDiscreteStates},
        };
}

bool EyeTrackingInputDevice::isEyeTrackingConnected()
{
    return plugin.eyeTrackingMgr->getEyeTrackingSupports() && plugin.eyeTrackingMgr->getEyeTrackingEnable();
}

int EyeTrackingInputDevice::getEyeTrackingId() { return device_EYES; }

InputDeviceBase* EyeTrackingInputDevice::getEyeTrackingDevice() { return new EyeTrackingInputDevice(); }

void EyeTrackingInputDevice::fillDeviceDefinition(UnityXRInputDeviceDefinition* definition)
{
    unityXRInputInterface->DeviceDefinition_SetName(definition, "EyesTracking");
    unityXRInputInterface->DeviceDefinition_SetManufacturer(definition, "YVR");
    unityXRInputInterface->DeviceDefinition_SetSerialNumber(definition, "qwe");
    unityXRInputInterface->DeviceDefinition_SetCharacteristics(definition, static_cast<UnityXRInputDeviceCharacteristics>(
                                                                   kUnityXRInputDeviceCharacteristicsTrackedDevice |
                                                                   kUnityXRInputDeviceCharacteristicsEyeTracking));
    for (const auto& pair : requiredUsages)
    {
        usage2IndexMap[pair.first] = unityXRInputInterface->DeviceDefinition_AddFeatureWithUsage(
            definition, pair.first, pair.second, pair.first);
    }
}

void EyeTrackingInputDevice::updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType /*updateType*/)
{
    updatePoseData(state);
}

void EyeTrackingInputDevice::updatePoseData(UnityXRInputDeviceState* state)
{
    auto const eyeTrackingLocation = plugin.eyeTrackingMgr->eyeGazeLocation;
    const bool isEyesPositionTracked = eyeTrackingLocation.locationFlags & XR_SPACE_LOCATION_POSITION_TRACKED_BIT;
    const bool isEyesOrientationTracked = eyeTrackingLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;
    SET_EYES_BINARY(unityXRInputInterface, state, kUnityXRInputFeatureUsageIsTracked, isEyesPositionTracked | isEyesOrientationTracked);

    UnityXREyes xrEyes = {};
    auto const position = OpenXRVector2UnityVector(eyeTrackingLocation.pose.position);
    auto const rotation = OpenXRQuaternion2UnityVector(eyeTrackingLocation.pose.orientation);

    xrEyes.fixationPoint = OpenXRVector2UnityVector(eyeTrackingLocation.pose.position);
    xrEyes.leftEyePose.position = xrEyes.rightEyePose.position = position;
    xrEyes.leftEyePose.rotation = xrEyes.rightEyePose.rotation = rotation;
    SET_EYES_VALUE(unityXRInputInterface, state, kUnityXRInputFeatureUsageEyesData, xrEyes);

    SET_EYE_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDevicePosition, eyeTrackingLocation.pose.position);
    SET_EYE_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceRotation, eyeTrackingLocation.pose.orientation);

    int trackingState = kUnityXRInputTrackingStateNone;
    if (isEyesPositionTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStatePosition);
    if (isEyesOrientationTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStateRotation);
    SET_EYES_State(unityXRInputInterface, state, kUnityXRInputFeatureUsageTrackingState, trackingState);
}
