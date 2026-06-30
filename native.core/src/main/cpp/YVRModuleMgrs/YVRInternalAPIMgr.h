#pragma once
#include <YvrLib_Api_Internal.h>

typedef void (*OnCameraDataCallback)(CAMERA_TYPE cameraType, camera_frame imgeData);

class YVRInternalAPIMgr
{
  public:
    YVRInternalAPIMgr();
    ~YVRInternalAPIMgr();
    OnCameraDataCallback onCameraDataCallback;

    static void onControllerServiceEventCallback(void* pCtx, yvrCameraFrame& state);

    void yvrRegistCameraDataCallBack(OnCameraDataCallback cameraCallback);
    void yvrUnregistCameraDataCallBack();

  private:
    static YVRInternalAPIMgr* pThis;
};
