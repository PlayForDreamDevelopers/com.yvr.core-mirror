#include "OpenXRAndroidPlatformPlugin.h"

#include "./Common.h"
#include "../YVRUnityPlugin.h"

std::vector<std::string> OpenXRAndroidPlatformPlugin::GetInstanceExtensions() const
{
    return {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME,XR_KHR_ANDROID_THREAD_SETTINGS_EXTENSION_NAME};
}

XrBaseInStructure* OpenXRAndroidPlatformPlugin::GetInstanceCreateInfo()
{
    instanceCreateInfoAndroid.applicationVM = plugin.javaVm;
    instanceCreateInfoAndroid.applicationActivity = plugin.activity;
    return (XrBaseInStructure*)&instanceCreateInfoAndroid;
}

OpenXRAndroidPlatformPlugin::OpenXRAndroidPlatformPlugin()
{
    // Initialize the loader for this platform
    PFN_xrInitializeLoaderKHR xrInitializeLoader = nullptr;
    const XrResult getProcResult = xrGetInstanceProcAddr(
        XR_NULL_HANDLE, "xrInitializeLoaderKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrInitializeLoader));
    if (XR_SUCCEEDED(getProcResult) && xrInitializeLoader != nullptr)
    {
        XrLoaderInitInfoAndroidKHR loaderInitInfoAndroid;
        loaderInitInfoAndroid.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
        loaderInitInfoAndroid.next = nullptr;
        loaderInitInfoAndroid.applicationVM = plugin.javaVm;
        loaderInitInfoAndroid.applicationContext = plugin.activity;
        const XrResult initResult = xrInitializeLoader(reinterpret_cast<const XrLoaderInitInfoBaseHeaderKHR*>(&loaderInitInfoAndroid));
        OpenXRResult(initResult, "xrInitializeLoaderKHR");
    }
    else
    {
        YError("xrInitializeLoaderKHR unavailable result=%s(%d) functionAvailable=%d",
               to_string(getProcResult), getProcResult, xrInitializeLoader != nullptr);
    }
}
