#include "YVRMeshMgr.h"
#include "../Utilities/YVRLog.h"
#include "../YVRUnityPlugin.h"
#include "../YVRUnityXR/YVRMeshingProvider.h"

YVRMeshMgr::YVRMeshMgr() {}
YVRMeshMgr::~YVRMeshMgr() {}

void YVRMeshMgr::createMeshDetectorYVR()
{
    AnnounceCallingFunc();
    XrMeshDetectorCreateInfoYVR meshDetectorCreateInfo{};
    meshDetectorCreateInfo.type = XR_TYPE_MESH_DETECTOR_CREATE_INFO_YVR;
    meshDetectorRuning = true;
    OpenXRAPI(xrCreateMeshDetectorYVR(plugin.openxrMgr->program->session, &meshDetectorCreateInfo, &meshDetector));
    AnnounceCalledFunc();
}

void YVRMeshMgr::destroyMeshDetectorYVR()
{
    AnnounceCallingFunc();
    meshDetectorRuning = false;
    if (plugin.unityXRProviderMgr != nullptr && plugin.unityXRProviderMgr->meshProvider != nullptr)
    {
        plugin.unityXRProviderMgr->meshProvider->clearMeshCache();
    }

    if (meshDetector != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrDestroyMeshDetectorYVR(meshDetector));
        meshDetector = XR_NULL_HANDLE;
    }

    AnnounceCalledFunc();
}

void YVRMeshMgr::beginRoomSetupYVR(RoomSetupBeginInfo roomSetupBeginInfo)
{
    AnnounceCallingFunc();
    const XrRoomSetupBeginInfoYVR info{
        XR_TYPE_ROOM_SETUP_BEGIN_INFO_YVR,
        nullptr,
        roomSetupBeginInfo.minRoomArea,
        roomSetupBeginInfo.maxRoomArea,
    };

    OpenXRAPI(xrBeginRoomSetupYVR(meshDetector, &info));
    AnnounceCalledFunc();
}

void YVRMeshMgr::endRoomSetupYVR()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrEndRoomSetupYVR(meshDetector));
    AnnounceCalledFunc();
}

void YVRMeshMgr::beginBoundaryDetect(float radius)
{
    AnnounceCallingFunc();
    const XrBoundaryDetectBeginInfoYVR boundaryDetectBeginInfo
    {
        XR_TYPE_BOUNDARY_DETECT_BEGIN_INFO_YVR,
        nullptr,
        radius,
    };

    OpenXRAPI(xrBeginBoundaryDetectYVR(meshDetector, &boundaryDetectBeginInfo));
}

void YVRMeshMgr::endBoundaryDetect()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrEndBoundaryDetectYVR(meshDetector));
}
