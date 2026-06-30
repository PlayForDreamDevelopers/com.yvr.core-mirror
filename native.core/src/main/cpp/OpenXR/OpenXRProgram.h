#pragma once

#include "./GraphicsPlugin/GraphicsPluginBase.h"
#include "./OpenXRAndroidPlatformPlugin.h"
#include "./pch.h"
#include "OpenXRExtMgr.h"

class OpenXRProgram
{
public:
    OpenXRProgram();

    void createInstance();
    void destroyInstance();

    void createSystem();

    void InitializeGfxDevice();
    void getViewConfigurations();

    void createSession();
    void destroySession();

    void BeginSession();
    void EndSession();

    XrInstance instance{XR_NULL_HANDLE};
    XrSession session{XR_NULL_HANDLE};
    XrSystemId systemId{XR_NULL_SYSTEM_ID};

    bool supportQuadViews{false};
    std::vector<XrViewConfigurationView> stereoViewsConfigs{};
    std::vector<XrViewConfigurationView> quadViewsConfigs{};

    std::vector<std::string> supportedExtensions{};

private:
    void createInstanceInternal();
    void enumerateSupportedExtensions();

    void getStereoViewsConfigs();
    void getQuadViewsConfigs();

    void LogLayersAndExtensions();
    void LogInstanceInfo();
};
