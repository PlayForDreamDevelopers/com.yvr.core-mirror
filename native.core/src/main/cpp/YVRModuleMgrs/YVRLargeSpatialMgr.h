#pragma once
#include <vector>
#include "openxr/openxr.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"

typedef struct YVRMarkerTrackingUpdate_
{
    uint64_t markerId;
    XrMarkerTypeYVR markerType;
    XrVector3f region[4];
    XrPosef markerPose;   // marker origin pose
    float confidence;
} YVRMarkerTrackingUpdate;

class YVRLargeSpatialMgr
{
  public:
    YVRLargeSpatialMgr();
    ~YVRLargeSpatialMgr();
    void setLargeSpaceMapRecordEnableYVR(XrBool32 enable);
    void setMarkerDetectionEnableYVR(XrBool32 enable);
    void clearAllMarkersYVR();
    void setLargeSpaceOriginPoseYVR(XrPosef inPose);
    void exportLargeSpaceMapYVR();
    void setRecenterEnableYVR(XrBool32 enable);
    XrBool32 getRecenterEnableYVR();
    void setLargeSpaceMapAssemblingEnable(XrBool32 enable);
    void getLargeSpacePointsCalibParams(XrPosef* outTransform, float* outGroundDistance);
};