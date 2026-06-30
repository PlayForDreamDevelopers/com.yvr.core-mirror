#pragma once
#include "openxr/openxr.h"
#include "../UnityHeader/XR/IUnityXRInput.h"

inline UnityXRTrackingOrigin OpenXRTrackingMode2Unity(const XrReferenceSpaceType& type)
{
    if (type == XR_REFERENCE_SPACE_TYPE_LOCAL)
        return kUnityXRInputTrackingOriginModeDevice;
    if (type == XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR)
        return kUnityXRInputTrackingOriginModeFloor;
    if (type == XR_REFERENCE_SPACE_TYPE_STAGE)
        return kUnityXRInputTrackingOriginModeTrackingReference;

    return kUnityXRInputTrackingOriginModeUnknown;
}

inline XrReferenceSpaceType UnityTrackingMode2OpenXR(const UnityXRTrackingOrigin& type)
{
    if (type == kUnityXRInputTrackingOriginModeDevice) return XR_REFERENCE_SPACE_TYPE_LOCAL;
    if (type == kUnityXRInputTrackingOriginModeFloor) return XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR;
    if (type == kUnityXRInputTrackingOriginModeTrackingReference) return XR_REFERENCE_SPACE_TYPE_STAGE;

    return XR_REFERENCE_SPACE_TYPE_MAX_ENUM;
}
