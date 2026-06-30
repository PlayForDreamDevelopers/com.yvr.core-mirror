#pragma once

#include "pfdm_openxr_preview/openxr_yvr_private.h"

typedef void (*OnHandCalibrationResultYVR)(int handCalibrationResult);

class YVRHandCalibrationMgr
{
public:
    void registerHandCalibrationEvent(OnHandCalibrationResultYVR callback);
    void unregisterHandCalibrationEvent();

private:
    static void onHandCalibrationEventCallback(EventHeaderYVR* evt);
    OnHandCalibrationResultYVR onHandCalibrationResult;
};
