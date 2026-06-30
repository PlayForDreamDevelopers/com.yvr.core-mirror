#include "YVRLargeSpatialMgr.h"
#include "../Utilities/YVRLog.h"
#include "../YVRUnityPlugin.h"

YVRLargeSpatialMgr::YVRLargeSpatialMgr() {}

YVRLargeSpatialMgr::~YVRLargeSpatialMgr() {}

void YVRLargeSpatialMgr::setLargeSpaceMapRecordEnableYVR(XrBool32 enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetLargeSpaceMapRecordEnableYVR(plugin.openxrMgr->program->session, enable));
}

void YVRLargeSpatialMgr::setMarkerDetectionEnableYVR(XrBool32 enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetMarkerDetectionEnableYVR(plugin.openxrMgr->program->session, enable));
}

void YVRLargeSpatialMgr::clearAllMarkersYVR()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrClearAllMarkersYVR(plugin.openxrMgr->program->session));
}

void YVRLargeSpatialMgr::setLargeSpaceOriginPoseYVR(XrPosef inPose)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetLargeSpaceOriginPoseYVR(plugin.openxrMgr->program->session, inPose));
}

void YVRLargeSpatialMgr::exportLargeSpaceMapYVR()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrExportLargeSpaceMapYVR(plugin.openxrMgr->program->session));
}

void YVRLargeSpatialMgr::setRecenterEnableYVR(XrBool32 enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetRecenterEnableYVR(plugin.openxrMgr->program->session, enable));
}

XrBool32 YVRLargeSpatialMgr::getRecenterEnableYVR()
{
    AnnounceCallingFunc();
    XrBool32 result;
    OpenXRAPI(xrGetRecenterEnableYVR(plugin.openxrMgr->program->session, &result));
    return result;
}

void YVRLargeSpatialMgr::setLargeSpaceMapAssemblingEnable(XrBool32 enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetLargeSpaceMapAssemblingEnableYVR(plugin.openxrMgr->program->session, enable));
}

void YVRLargeSpatialMgr::getLargeSpacePointsCalibParams(XrPosef* outTransform, float* outGroundDistance)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetLargeSpacePointsCalibParamsYVR(plugin.openxrMgr->program->session, outTransform, outGroundDistance));
}