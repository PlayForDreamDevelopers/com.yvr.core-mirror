#pragma once

#include "InputDeviceBase.h"
#include "../../YVRModuleMgrs/Inputs/XRMotionData.h"

static constexpr int device_EYES = 2000;

class EyeTrackingInputDevice : public InputDeviceBase
{
public:
    static bool isEyeTrackingConnected();
    static int getEyeTrackingId();
    static InputDeviceBase* getEyeTrackingDevice();

    EyeTrackingInputDevice();

    void fillDeviceDefinition(UnityXRInputDeviceDefinition* definition) override;
    void updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType) override;

private:
    void updatePoseData(UnityXRInputDeviceState* state);
};
