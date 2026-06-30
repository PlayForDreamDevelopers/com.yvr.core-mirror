#pragma once
#include "InputDeviceBase.h"
#include "../../YVRModuleMgrs/Inputs/XRMotionData.h"
#include "../../YVRModuleMgrs/Inputs/Controllers/ControllerXRActionContext.h"

static constexpr int device_LEFT_CONTROLLER = 1001;
static constexpr int device_RIGHT_CONTROLLER = 1002;

class ControllerInputDevice : public InputDeviceBase
{
public:
    static bool isLeftControllerConnected() { return isConnected(LEFT); }
    static int getLeftControllerId() { return device_LEFT_CONTROLLER; }
    static InputDeviceBase* getLeftControllerDevice() { return new ControllerInputDevice(LEFT); }

    static bool isRightControllerConnected() { return isConnected(RIGHT); }
    static int getRightControllerId() { return device_RIGHT_CONTROLLER; }
    static InputDeviceBase* getRightControllerDevice() { return new ControllerInputDevice(RIGHT); }

    explicit ControllerInputDevice(ControllerSide side);

    void fillDeviceDefinition(UnityXRInputDeviceDefinition* definition) override;
    void updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType) override;

private:
    void updatePoseData(UnityXRInputDeviceState* state, const XRMotionData* controllerPose, XrSpaceLocationFlags locationFlags);
    void updateActionState(int side, UnityXRInputDeviceState* state);
    void updateBatteryState(int side, UnityXRInputDeviceState* state);
    static bool isConnected(ControllerSide side);
    ControllerSide side{};
};
