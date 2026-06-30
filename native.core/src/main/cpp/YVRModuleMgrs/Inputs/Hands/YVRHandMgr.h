#pragma once

#include <cstdint>
#include "openxr/openxr.h"
#include "XRHandData.h"

#define HAND_POSE_LENGTH 2000

class YVRHandMgr
{
public:
    XRHandData handDataL{};
    XRHandData handDataR{};

    bool isHandTrackingEnable = false;
    bool isInit = false;

    YVRHandMgr();
    ~YVRHandMgr();
    void initHandTracking();
    void setHandTrackingEnable(bool enabled);
    void refreshHandPose(bool forRender);

private:
    XrHandTrackerEXT handTrackerL = XR_NULL_HANDLE;
    XrHandTrackerEXT handTrackerR = XR_NULL_HANDLE;
};
