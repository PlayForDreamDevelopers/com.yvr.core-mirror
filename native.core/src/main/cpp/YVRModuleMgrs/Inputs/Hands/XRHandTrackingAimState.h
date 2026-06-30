#pragma once

#include "openxr/openxr.h"

typedef struct XRHandTrackingAimState_
{
    XrHandTrackingAimFlagsFB status;
    XrPosef aimPose;
    float pinchStrengthIndex;
    float pinchStrengthMiddle;
    float pinchStrengthRing;
    float pinchStrengthLittle;
} XRHandTrackingAimState;
