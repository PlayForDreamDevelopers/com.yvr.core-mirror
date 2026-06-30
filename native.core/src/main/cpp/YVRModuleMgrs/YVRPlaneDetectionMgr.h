#pragma once
#include <vector>
#include "openxr/openxr.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"

typedef struct YVRPlaneDetectorPolygonBuffers_
{
    uint32_t polygonCount;
    XrPlaneDetectorPolygonBufferYVR* polygonBuffer;
}YVRPlaneDetectorPolygonBuffers;


typedef void (*OnGetPlaneDetections)(XrPlaneDetectorLocationsYVR planeDetectorLocations);

class YVRPlaneDetectionMgr
{
  private:
    void queryPlaneDetectionsInfo();

  public:
    YVRPlaneDetectionMgr();
    ~YVRPlaneDetectionMgr();

    void createPlaneDetection();
    void getPlaneDetections();
    YVRPlaneDetectorPolygonBuffers* getPolygonBuffer(uint64_t planeId, int32_t count);
    void setPlaneDetectionsCallback(OnGetPlaneDetections callback);
    void destoryPlaneDetection();

    XrMeshDetectorYVR meshDetector;
    bool planeDetectorRuning = false;
    OnGetPlaneDetections onGetPlaneDetections = nullptr;
    XrPlaneDetectorLocationsYVR planeDetectorLocations{XR_TYPE_PLANE_DETECTOR_LOCATIONS_YVR};
    YVRPlaneDetectorPolygonBuffers polygonBuffers {};
    std::vector<XrPlaneDetectorLocationYVR> locationsBuffer;
    std::vector<XrPlaneDetectorPolygonBufferYVR> tempBuffers;
    std::vector<XrVector2f> vertices;
};