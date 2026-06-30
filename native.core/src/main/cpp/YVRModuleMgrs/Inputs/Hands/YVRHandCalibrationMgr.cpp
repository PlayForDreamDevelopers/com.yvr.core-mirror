#include "YVRHandCalibrationMgr.h"

#include "openxr_yvr_private.h"
#include "../../../Utilities/YVRLog.h"
#include "../../../YVRUnityPlugin.h"

void YVRHandCalibrationMgr::registerHandCalibrationEvent(OnHandCalibrationResultYVR callback)
{
    AnnounceCallingFunc();
    onHandCalibrationResult = callback;
    OpenXRAPI(xrRegisterHandEventCallbackYVR(plugin.openxrMgr->program->session, onHandCalibrationEventCallback));
}

void YVRHandCalibrationMgr::unregisterHandCalibrationEvent()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrUnregisterHandEventCallbackYVR(plugin.openxrMgr->program->session));
}

void YVRHandCalibrationMgr::onHandCalibrationEventCallback(EventHeaderYVR* evt)
{
    YDebug("Get HandCalibrationEvent %d", evt->type);
    if (plugin.handCalibrationMgr->onHandCalibrationResult != nullptr)
        plugin.handCalibrationMgr->onHandCalibrationResult(evt->type);
}
