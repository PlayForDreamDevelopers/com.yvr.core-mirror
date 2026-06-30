#pragma once
#include "../../OpenXR/pch.h"

class XRMotionData
{
public:
    XRMotionData();
    XrSpaceAccelerationExtYVR acceleration;
    XrSpaceVelocity velocity;
    XrSpaceLocation location;
};
