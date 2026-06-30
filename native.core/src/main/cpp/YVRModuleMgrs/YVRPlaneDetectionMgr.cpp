#include "YVRPlaneDetectionMgr.h"
#include "../Utilities/YVRLog.h"
#include "../Utilities/YVRCoordinateConverter.h"
#include "../YVRUnityPlugin.h"

YVRPlaneDetectionMgr::YVRPlaneDetectionMgr() {}

void YVRPlaneDetectionMgr::createPlaneDetection()
{
    AnnounceCallingFunc();
    XrMeshDetectorCreateInfoYVR meshDetectorCreateInfo{XR_TYPE_MESH_DETECTOR_CREATE_INFO_YVR};
    XrPlaneDetectorCreateInfoYVR createInfoPlane{XR_TYPE_PLANE_DETECTOR_CREATE_INFO_YVR};
    meshDetectorCreateInfo.next = &createInfoPlane;
    planeDetectorRuning = true;
    OpenXRAPI(xrCreateMeshDetectorYVR(plugin.openxrMgr->program->session, &meshDetectorCreateInfo, &meshDetector));
    AnnounceCalledFunc();
}

void YVRPlaneDetectionMgr::setPlaneDetectionsCallback(OnGetPlaneDetections callback)
{
    this->onGetPlaneDetections = callback;
}

void YVRPlaneDetectionMgr::getPlaneDetections()
{
    AnnounceCallingFunc();
    XrPlaneDetectorGetInfoYVR getInfoYvr{XR_TYPE_PLANE_DETECTOR_GET_INFO_YVR};
    getInfoYvr.baseSpace = plugin.inputMgr->appSpace;
    planeDetectorLocations.planeLocationCapacityInput = 0;
    OpenXRAPI(xrGetPlaneDetectionsYVR(meshDetector, &getInfoYvr, &planeDetectorLocations));
    locationsBuffer.resize(planeDetectorLocations.planeLocationCountOutput);
    if (planeDetectorLocations.planeLocationCountOutput > 0)
    {
        planeDetectorLocations.type = XR_TYPE_PLANE_DETECTOR_LOCATION_EXT;
        planeDetectorLocations.planeLocationCapacityInput = planeDetectorLocations.planeLocationCountOutput;
        planeDetectorLocations.planeLocations = locationsBuffer.data();
        OpenXRAPI(xrGetPlaneDetectionsYVR(meshDetector, &getInfoYvr, &planeDetectorLocations));
        for (size_t i = 0; i < planeDetectorLocations.planeLocationCapacityInput; i++)
        {
            planeDetectorLocations.planeLocations[i].pose = ConvertPoseBetweenOpenXRandUnity(planeDetectorLocations.planeLocations[i].pose);
        }
    }

    if (onGetPlaneDetections != nullptr)
        onGetPlaneDetections(planeDetectorLocations);

    AnnounceCalledFunc();
}

YVRPlaneDetectorPolygonBuffers* YVRPlaneDetectionMgr::getPolygonBuffer(uint64_t planeId, int32_t count)
{
    tempBuffers.resize(count);
    for (uint32_t polygonBufferIndex = 0; polygonBufferIndex < count; polygonBufferIndex++)
    {
        XrPlaneDetectorPolygonBufferYVR& polygonBuffer = tempBuffers[polygonBufferIndex];
        polygonBuffer.type = XR_TYPE_PLANE_DETECTOR_POLYGON_BUFFER_YVR;
        polygonBuffer.vertexCapacityInput = 0;
        polygonBuffer.vertexCountOutput = 0;
        OpenXRAPI(xrGetPlanePolygonBufferYVR(meshDetector, planeId, polygonBufferIndex, &polygonBuffer));
        vertices.resize(polygonBuffer.vertexCountOutput);
        polygonBuffer.vertexCapacityInput = polygonBuffer.vertexCountOutput;
        polygonBuffer.vertices = vertices.data();
        OpenXRAPI(xrGetPlanePolygonBufferYVR(meshDetector, planeId, polygonBufferIndex, &polygonBuffer));
    }

    polygonBuffers.polygonCount = count;
    polygonBuffers.polygonBuffer = tempBuffers.data();
    return &polygonBuffers;
}

void YVRPlaneDetectionMgr::destoryPlaneDetection()
{
    AnnounceCallingFunc();
    if (meshDetector != XR_NULL_HANDLE)
    {
        planeDetectorRuning = false;
        OpenXRAPI(xrDestroyMeshDetectorYVR(meshDetector));
        AnnounceCalledFunc();
    }
}
