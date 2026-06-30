#pragma once

#include "../XRMotionData.h"

class YVRHMDMgr
{
public:
    void recenterPose();
    void getRecenterOffset(XrPosef* pose);
    void getRecenterPose(XrPosef* pose);
    void getStageSpacePose(XrPosef* pose);

    bool isUserPresent();
    XRMotionData hmdRenderPose{};
    XRMotionData hmdPhysicsPose{};

    XrPosef lEyeRenderPose{}, rEyeRenderPose{};
    XrPosef lEyePhysicsPose{}, rEyePhysicsPose{};

    std::vector<XrView> physicalViews{}, renderViews{};

    void refreshViews(const bool forRender);
    void refreshHmdPose(const bool forRender);
    XrPosef getPredictedTimePose(int64_t time);
};
