#pragma once

#include <pfdm_openxr_preview/openxr_yvr_private.h>

struct EyeXRActionContext
{
    XrActionSet actionSet{XR_NULL_HANDLE};
    XrSpace eyeGazeSpace;
    XrAction eyeGazePoseAction{XR_NULL_HANDLE};
};
