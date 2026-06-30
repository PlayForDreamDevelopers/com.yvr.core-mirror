#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <openxr.h>
#include <stdio.h>
#include <unistd.h>

#include <mutex>
#include <string>
#include "YVRLog.h"

using std::string;

#define CHK_STRINGIFY(x) #x
#define TOSTRING(x) CHK_STRINGIFY(x)
#define FILE_AND_LINE __FILE__ ":" TOSTRING(__LINE__)

#define GL(func) \
    func;        \
    CheckGLError(__FILE__, __LINE__);

#define CHECK(exp)                                      \
    {                                                   \
        if (!(exp))                                     \
        {                                               \
            YError("Check failed", #exp, FILE_AND_LINE); \
        }                                               \
    }


#define VRAPI(func)                                                                                                      \
    {                                                                                                                    \
        int result = func;                                                                                               \
        if (result != 0)                                                                                                 \
        {                                                                                                                \
            YError("VR API ERROR: function: %s with result %d, in %s : %d\n", __FUNCTION__, result, __FILE__, __LINE__); \
        }                                                                                                                \
    }

#define AnnounceCallingFunc() YInfo("Called %s", __FUNCTION__);
#define AnnounceDirectlyReturn(reason) YInfo("Directly %s for %s", __FUNCTION__,reason);

#define AnnounceCalledFunc() YInfo("Called %s end", __FUNCTION__);

#define UnityXRAPI(func)                                                                                                  \
    {                                                                                                                     \
        UnitySubsystemErrorCode result = func;                                                                            \
        if (result != kUnitySubsystemErrorCodeSuccess)                                                                    \
        {                                                                                                                 \
            YError("Unity SUBSYSTEM ERROR:%d within function: %s, in %s: %d,", result, __FUNCTION__, __FILE__, __LINE__); \
        }                                                                                                                 \
    }

#define OpenXRAPI(cmd) CheckXrResult(cmd, #cmd, FILE_AND_LINE);
#define OpenXRResult(res,cmdStr) CheckXrResult(res,cmdStr,FILE_AND_LINE);

void CheckGLError(const char* file, int line);

XrResult CheckXrResult(XrResult res, const char* originator = nullptr, const char* sourceLocation = nullptr);
void CheckEGLError(const char* file, int line);
