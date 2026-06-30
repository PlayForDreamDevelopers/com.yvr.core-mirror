#pragma once

#include "openxr_yvr_private.h"
#include "openxr/openxr.h"

class YVRInputMgr
{
public:
    YVRInputMgr();
    ~YVRInputMgr();

    void refreshRenderPoseData();
    void refreshPhysicsPoseData();

    XrTime getTime(const bool forRender);

    void setTrackingSpace(XrReferenceSpaceType trackingSpace);
    XrReferenceSpaceType getTrackingSpace();

    void getCurrentInputDevice(XrInputDeviceFlagsYVR* inputDevice);

    void createAppSpace();
    void createViewSpace();
    void createLocalSpace();
    void createStageSpace();

    void destroyLocalSpace();
    void destroyStageSpace();
    void destroyAppSpace();
    void destroyViewSpace();

    XrSpace& GetAppSpace() { return appSpace; }
    void createActions();
    void destroyActions();
    void refreshActionState();

    XrSpace appSpace{};
    XrSpace viewSpace{};
    XrSpace localSpace{};
    XrSpace stageSpace{};
    bool isBlockingInteractionData = false;
    XrReferenceSpaceType trackingSpace{XR_REFERENCE_SPACE_TYPE_LOCAL};
};
