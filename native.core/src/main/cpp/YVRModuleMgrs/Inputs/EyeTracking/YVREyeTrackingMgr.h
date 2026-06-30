#pragma once

#include "openxr/openxr.h"
#include "./EyeXRActionContext.h"

class YVREyeTrackingMgr
{
public:
    YVREyeTrackingMgr();
    ~YVREyeTrackingMgr();
    bool getEyeTrackingEnable();
    bool getEyeTrackingSupports();

    void createActions();
    void updateEyeTrackingPose(const bool forRender);
    void destroyActions();

    EyeXRActionContext actionCtx{};
    XrSpaceLocation eyeGazeLocation;

private:
    XrSystemEyeGazeInteractionPropertiesEXT eyeGazeInteractionProperties;
    XrSystemProperties systemProperties;

    XrEyeGazeSampleTimeEXT eyeGazeSampleTime;
};
