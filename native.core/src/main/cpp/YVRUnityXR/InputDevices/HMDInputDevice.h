#pragma once
#include "InputDeviceBase.h"
#include "../../YVRModuleMgrs/Inputs/XRMotionData.h"

static constexpr int device_HMD = 0;

class HMDInputDevice : public InputDeviceBase
{
public:
    static bool isHMDConnected() { return true; }
    static int getHMDId() { return device_HMD; }
    static InputDeviceBase* getHMDDevice() { return new HMDInputDevice(); }

    HMDInputDevice();

    void fillDeviceDefinition(UnityXRInputDeviceDefinition* definition) override;
    void updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType) override;

private:
    void updatePoseData(UnityXRInputDeviceState* state, const XRMotionData* hmdPose, const XrPosef* leftEyePose, const XrPosef* rightEyePose);
};
