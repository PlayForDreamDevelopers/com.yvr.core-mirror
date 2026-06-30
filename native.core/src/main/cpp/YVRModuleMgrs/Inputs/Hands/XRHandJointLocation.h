#pragma once

#include "XRHandTrackingAimState.h"

typedef struct XRHandJointLocations_
{
    uint32_t isActive;
    uint32_t jointCount;
    float handScale;
    XRHandTrackingAimState aimState{};
} XRHandJointLocations;
