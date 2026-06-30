#pragma once

#include "pch.h"

class OpenXRAndroidPlatformPlugin
{
public:
    OpenXRAndroidPlatformPlugin();
    std::vector<std::string> GetInstanceExtensions() const;
    XrBaseInStructure* GetInstanceCreateInfo();

private:
    XrInstanceCreateInfoAndroidKHR instanceCreateInfoAndroid{XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
};
