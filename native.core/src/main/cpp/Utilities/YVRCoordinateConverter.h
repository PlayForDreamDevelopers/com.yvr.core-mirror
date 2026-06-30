#pragma once

#include "openxr/openxr.h"
#include "../UnityHeader/XR/UnityXRTypes.h"

inline UnityXRVector3 OpenXRVector2UnityVector(const XrVector3f& vector)
{
    return UnityXRVector3{vector.x, vector.y, -vector.z};
}

inline UnityXRVector4 OpenXRQuaternion2UnityVector(const XrQuaternionf& quaternion)
{
    return UnityXRVector4{-quaternion.x, -quaternion.y, quaternion.z, quaternion.w};
}

inline UnityXRPose OpenXRPose2UnityPose(const XrPosef& posef)
{
    return UnityXRPose{OpenXRVector2UnityVector(posef.position), OpenXRQuaternion2UnityVector(posef.orientation)};
}

inline XrVector3f ConvertPoseBetweenOpenXRandUnity(const XrVector3f& vector)
{
    return XrVector3f{vector.x, vector.y, -vector.z};
}

inline XrQuaternionf ConvertPoseBetweenOpenXRandUnity(const XrQuaternionf& quaternion)
{
    return XrQuaternionf{-quaternion.x, -quaternion.y, quaternion.z, quaternion.w};
}

inline XrPosef ConvertPoseBetweenOpenXRandUnity(const XrPosef& posef)
{
    return XrPosef{ConvertPoseBetweenOpenXRandUnity(posef.orientation), ConvertPoseBetweenOpenXRandUnity(posef.position),};
}
