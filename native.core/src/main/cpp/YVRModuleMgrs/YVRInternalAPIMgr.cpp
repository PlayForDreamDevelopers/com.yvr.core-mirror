#include "YVRInternalAPIMgr.h"
#include "../Utilities/YVRLog.h"

YVRInternalAPIMgr* YVRInternalAPIMgr::pThis = NULL;

YVRInternalAPIMgr::YVRInternalAPIMgr() : onCameraDataCallback(nullptr) { pThis = this; }

void YVRInternalAPIMgr::yvrRegistCameraDataCallBack(OnCameraDataCallback cameraCallback)
{
    onCameraDataCallback = cameraCallback;
    yvrlib_internal_RegisterCameraFrameCallback(onControllerServiceEventCallback);
    YInfo("yvrRegistCameraDataCallBack end");
}

void YVRInternalAPIMgr::yvrUnregistCameraDataCallBack()
{
    onCameraDataCallback = NULL;
    yvrlib_internal_UnregisterCameraFrameCallback();
    YInfo("yvrUnregistCameraDataCallBack end");
}

void YVRInternalAPIMgr::onControllerServiceEventCallback(void* pCtx, yvrCameraFrame& state)
{
    if (pThis->onCameraDataCallback != nullptr) pThis->onCameraDataCallback(state.type, state.frame);
}