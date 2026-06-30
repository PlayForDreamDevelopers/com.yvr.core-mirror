#include "ControllerInputDevice.h"

#include "../../Utilities/YVRCoordinateConverter.h"
#include "../../YVRUnityPlugin.h"

UnityXRInputFeatureUsage const kUnityXRInputFeatureUsageCharging = "BatteryCharging";

#define SET_CTRL_VEC3(inputInterface, state, usage, data) \
inputInterface->DeviceState_SetAxis3DValue(state, usage2IndexMap[usage],OpenXRVector2UnityVector(data));

#define SET_CTRL_ROTATION(inputInterface, state, usage, data) \
inputInterface->DeviceState_SetRotationValue(state, usage2IndexMap[usage], OpenXRQuaternion2UnityVector(data));

#define SET_CTRL_BINARY(inputInterface, state, usage, data) \
inputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[usage], data);

#define SET_CTRL_State(inputInterface, state, usage, data) \
inputInterface->DeviceState_SetDiscreteStateValue(state, usage2IndexMap[usage], data);

#define SET_CTRL_BINARY_ACTION(inputInterface, state, usage, isLeft, action) \
inputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[usage], plugin.controllersMgr->getBooleanActionValue(isLeft, action));

bool ControllerInputDevice::isConnected(const ControllerSide side)
{
    return plugin.controllersMgr->getActiveValue(side);
}


ControllerInputDevice::ControllerInputDevice(const ControllerSide side)
    : side(side)
{
    requiredUsages = {
            {kUnityXRInputFeatureUsageDevicePosition, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceRotation, kUnityXRInputFeatureTypeRotation},
            {kUnityXRInputFeatureUsageDeviceVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAngularVelocity, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsageDeviceAngularAcceleration, kUnityXRInputFeatureTypeAxis3D},
            {kUnityXRInputFeatureUsagePrimaryButton, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsagePrimaryTouch, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageSecondaryButton, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageSecondaryTouch, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageTriggerButton, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageIndexTouch, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageTrigger, kUnityXRInputFeatureTypeAxis1D},
            {kUnityXRInputFeatureUsageGripButton, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageGrip, kUnityXRInputFeatureTypeAxis1D},
            {kUnityXRInputFeatureUsageMenuButton, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsagePrimary2DAxisClick, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsagePrimary2DAxis, kUnityXRInputFeatureTypeAxis2D},
            {kUnityXRInputFeatureUsagePrimary2DAxisTouch, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageBatteryLevel, kUnityXRInputFeatureTypeAxis1D},
            {kUnityXRInputFeatureUsageCharging, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageIsTracked, kUnityXRInputFeatureTypeBinary},
            {kUnityXRInputFeatureUsageTrackingState, kUnityXRInputFeatureTypeDiscreteStates},
        };
}

void ControllerInputDevice::fillDeviceDefinition(UnityXRInputDeviceDefinition* definition)
{
    unityXRInputInterface->DeviceDefinition_SetName(definition, side == LEFT ? "LeftController" : "RightController");
    unityXRInputInterface->DeviceDefinition_SetManufacturer(definition, "YVR");

    auto deviceCharacteristics = kUnityXRInputDeviceCharacteristicsTrackedDevice | kUnityXRInputDeviceCharacteristicsController |
                                 kUnityXRInputDeviceCharacteristicsHeldInHand;
    deviceCharacteristics |= side == LEFT ? kUnityXRInputDeviceCharacteristicsLeft : kUnityXRInputDeviceCharacteristicsRight;

    unityXRInputInterface->DeviceDefinition_SetCharacteristics(definition, static_cast<UnityXRInputDeviceCharacteristics>(deviceCharacteristics));

    for (const auto& pair : requiredUsages)
    {
        usage2IndexMap[pair.first] = unityXRInputInterface->DeviceDefinition_AddFeatureWithUsage(
            definition, pair.first, pair.second, pair.first);
    }
}


void ControllerInputDevice::updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType)
{
    static const XRMotionData* const leftRenderPose = &plugin.controllersMgr->leftRenderPose;
    static const XRMotionData* const leftPhysicalPose = &plugin.controllersMgr->leftPhysicsPose;
    static const XRMotionData* const rightRenderPose = &plugin.controllersMgr->rightRenderPose;
    static const XRMotionData* const rightPhysicalPose = &plugin.controllersMgr->rightPhysicsPose;

    if (plugin.controllersMgr->isControllerEnable)
    {
        const bool isDynamic = updateType == kUnityXRInputUpdateTypeDynamic;
        const XRMotionData* pose = isDynamic ? (side == LEFT ? leftPhysicalPose : rightPhysicalPose) :
                                       (side == LEFT ? leftRenderPose : rightRenderPose);
        const XrSpaceLocationFlags locationFlags = isDynamic ? (side == LEFT ? leftPhysicalPose->location.locationFlags
                                                                    : rightPhysicalPose->location.locationFlags)
                                                       : (side == LEFT ? leftRenderPose->location.locationFlags
                                                              : rightRenderPose->location.locationFlags);

        updatePoseData(state, pose, locationFlags);
        updateActionState(side, state);
    }

    updateBatteryState(side, state);
}

void ControllerInputDevice::updatePoseData(UnityXRInputDeviceState* state, const XRMotionData* controllerPose, XrSpaceLocationFlags locationFlags)
{
    const bool isControllerPositionTracked = locationFlags & XR_SPACE_LOCATION_POSITION_TRACKED_BIT;
    const bool isControllerOrientationTracked = locationFlags & XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;

    SET_CTRL_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDevicePosition, controllerPose->location.pose.position);
    SET_CTRL_ROTATION(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceRotation, controllerPose->location.pose.orientation);
    SET_CTRL_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceVelocity, controllerPose->velocity.linearVelocity);
    SET_CTRL_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAngularVelocity, controllerPose->velocity.angularVelocity);
    SET_CTRL_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAcceleration, controllerPose->acceleration.linearAcceleration);
    SET_CTRL_VEC3(unityXRInputInterface, state, kUnityXRInputFeatureUsageDeviceAngularAcceleration, controllerPose->acceleration.angularAcceleration);
    SET_CTRL_BINARY(unityXRInputInterface, state, kUnityXRInputFeatureUsageIsTracked, isControllerPositionTracked | isControllerOrientationTracked);

    int trackingState = kUnityXRInputTrackingStateNone;
    if (isControllerPositionTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStatePosition);
    if (isControllerOrientationTracked) trackingState |= static_cast<int>(kUnityXRInputTrackingStateRotation);

    SET_CTRL_State(unityXRInputInterface, state, kUnityXRInputFeatureUsageTrackingState, trackingState);
}

void ControllerInputDevice::updateActionState(const int side, UnityXRInputDeviceState* state)
{
    const bool isLeft = side == LEFT;
    const ControllerXRActionContext& inputState = plugin.controllersMgr->actionCtx;

    const UnityXRVector2 thumbstickValue{plugin.controllersMgr->getFloatActionValue(isLeft, inputState.thumbstickXValue),
                                         plugin.controllersMgr->getFloatActionValue(isLeft, inputState.thumbstickYValue)};
    const float gripValue = plugin.controllersMgr->getBooleanActionValue(isLeft, inputState.gripButtonAction) ? 1.0f : 0.0f;

    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsagePrimaryButton, isLeft, inputState.primaryButtonAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsagePrimaryTouch, isLeft, inputState.primaryTouchAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsageSecondaryButton, isLeft, inputState.secondaryButtonAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsageSecondaryTouch, isLeft, inputState.secondaryTouchAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsageIndexTouch, isLeft, inputState.triggerTouchAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsageGripButton, isLeft, inputState.gripButtonAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsageMenuButton, isLeft, inputState.menuButtonAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsagePrimary2DAxisClick, isLeft, inputState.thumbstickButtonAction);
    SET_CTRL_BINARY_ACTION(unityXRInputInterface, state, kUnityXRInputFeatureUsagePrimary2DAxisTouch, isLeft, inputState.thumbstickTouchAction);

    unityXRInputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[kUnityXRInputFeatureUsageTriggerButton],
                                                      plugin.controllersMgr->getFloatActionValue(isLeft, inputState.triggerValue) > 0.5f);
    unityXRInputInterface->DeviceState_SetAxis1DValue(state, usage2IndexMap[kUnityXRInputFeatureUsageGrip], gripValue);
    unityXRInputInterface->DeviceState_SetAxis1DValue(state, usage2IndexMap[kUnityXRInputFeatureUsageTrigger],
                                                      plugin.controllersMgr->getFloatActionValue(isLeft, inputState.triggerValue));
    unityXRInputInterface->DeviceState_SetAxis2DValue(state, usage2IndexMap[kUnityXRInputFeatureUsagePrimary2DAxis], thumbstickValue);
}

void ControllerInputDevice::updateBatteryState(const int side, UnityXRInputDeviceState* state)
{
    unityXRInputInterface->DeviceState_SetBinaryValue(state, usage2IndexMap[kUnityXRInputFeatureUsageCharging],
                                                      plugin.controllersMgr->actionCtx.ctrlChargeStatus[side].isCharging == 1);
    unityXRInputInterface->DeviceState_SetAxis1DValue(state, usage2IndexMap[kUnityXRInputFeatureUsageBatteryLevel],
                                                      plugin.controllersMgr->actionCtx.ctrlChargeStatus[side].batteryPercentRemaining / 100.0f);
}
