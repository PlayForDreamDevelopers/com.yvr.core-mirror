#pragma once
#include <vector>
#include "openxr/openxr.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"

typedef struct RoomSetupBeginInfo_
{
    float                       minRoomArea;
    float                       maxRoomArea;
} RoomSetupBeginInfo;

class YVRMeshMgr
{
  private:

    XrMeshDetectorYVR meshDetector = XR_NULL_HANDLE;

  public:
    YVRMeshMgr();
    ~YVRMeshMgr();
    void createMeshDetectorYVR();
    void destroyMeshDetectorYVR();
    void beginRoomSetupYVR(RoomSetupBeginInfo roomSetupBeginInfo);
    void endRoomSetupYVR();
    void beginBoundaryDetect(float radius);
    void endBoundaryDetect();

    bool meshDetectorRuning = false;
};