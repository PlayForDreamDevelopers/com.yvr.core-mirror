#pragma once

#include "../OpenXR/OpenXRProgram.h"
#include "../OpenXR/pch.h"
#include "../OpenXR/GraphicsPlugin/GraphicsPluginOpenGLES.h"

class YVROpenXRMgr
{
public:
    YVROpenXRMgr();
    void selectGraphicPlugin(IUnityInterfaces* interfaces);
    void initialize();
    UnityGfxRenderer renderer = kUnityGfxRendererNull;
    std::shared_ptr<OpenXRProgram> program;
    std::shared_ptr<OpenXRExtMgr> extMgr;
    std::shared_ptr<GraphicsPluginBase> graphicsPlugin{};
    std::shared_ptr<OpenXRAndroidPlatformPlugin> platformPlugin{};
};
