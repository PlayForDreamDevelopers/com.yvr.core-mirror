#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#ifdef XR_USE_PLATFORM_ANDROID
#include <android/log.h>
#include <android/native_window.h>
#include <jni.h>
#include <sys/system_properties.h>
#endif

#ifdef XR_USE_GRAPHICS_API_OPENGL_ES
#include <EGL/egl.h>
#endif

#ifdef XR_USE_GRAPHICS_API_VULKAN
#ifdef XR_USE_PLATFORM_ANDROID
#define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include <vulkan/vulkan.h>
#endif

#include <openxr/openxr.h>
#include <pfdm_openxr_preview/openxr_yvr_private.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>

#include "../Utilities/YVRDebug.h"


