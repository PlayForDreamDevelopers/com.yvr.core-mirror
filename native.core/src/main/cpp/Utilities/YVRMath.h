#pragma once

#include "openxr/openxr.h"

inline XrQuaternionf XrQuaternionf_Inverse(const XrQuaternionf& q)
{
    XrQuaternionf r;
    r.x = -q.x;
    r.y = -q.y;
    r.z = -q.z;
    r.w = q.w;
    return r;
}

inline void XrVector3f_Add(XrVector3f* result, const XrVector3f* a, const XrVector3f* b)
{
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

inline XrVector3f XrVector3f_ScalarMultiply(const XrVector3f& v, float scale)
{
    XrVector3f u;
    u.x = v.x * scale;
    u.y = v.y * scale;
    u.z = v.z * scale;
    return u;
}

inline void XrQuaternionf_Multiply(XrQuaternionf& result, const XrQuaternionf& a, const XrQuaternionf& b)
{
    result.x = (b.w * a.x) + (b.x * a.w) + (b.y * a.z) - (b.z * a.y);
    result.y = (b.w * a.y) - (b.x * a.z) + (b.y * a.w) + (b.z * a.x);
    result.z = (b.w * a.z) + (b.x * a.y) - (b.y * a.x) + (b.z * a.w);
    result.w = (b.w * a.w) - (b.x * a.x) - (b.y * a.y) - (b.z * a.z);
}

inline XrVector3f XrQuaternionf_Rotate(const XrQuaternionf& a, const XrVector3f& v)
{
    XrQuaternionf q = {v.x, v.y, v.z, 0.0f};
    XrQuaternionf aq;
    XrQuaternionf_Multiply(aq, q, a);
    XrQuaternionf aInv = XrQuaternionf_Inverse(a);
    XrQuaternionf aqaInv;
    XrQuaternionf_Multiply(aqaInv, aInv, aq);
    return {aqaInv.x, aqaInv.y, aqaInv.z};
}

inline XrVector3f XrPosef_Transform(const XrPosef& a, const XrVector3f& v)
{
    XrVector3f r0 = XrQuaternionf_Rotate(a.orientation, v);
    XrVector3f result;
    XrVector3f_Add(&result, &r0, &a.position);
    return result;
}

inline XrPosef XrPosef_Inverse(const XrPosef& a)
{
    XrPosef b;
    b.orientation = XrQuaternionf_Inverse(a.orientation);
    b.position = XrQuaternionf_Rotate(b.orientation, XrVector3f_ScalarMultiply(a.position, -1.0f));
    return b;
}

inline XrPosef XrPosef_Multiply(const XrPosef& a, const XrPosef& b)
{
    XrPosef c;
    XrQuaternionf_Multiply(c.orientation, b.orientation, a.orientation);
    c.position = XrPosef_Transform(a, b.position);
    return c;
}
