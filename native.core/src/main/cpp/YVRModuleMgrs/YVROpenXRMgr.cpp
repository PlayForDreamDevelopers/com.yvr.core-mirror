#include "./YVROpenXRMgr.h"
#include "../YVRUnityPlugin.h"

#include "../OpenXR/OpenXRAndroidPlatformPlugin.h"
#include "../OpenXR/GraphicsPlugin/GraphicsPluginVulkan.h"

YVROpenXRMgr::YVROpenXRMgr() = default;

void YVROpenXRMgr::initialize()
{
    AnnounceCallingFunc();

    platformPlugin = std::make_shared<OpenXRAndroidPlatformPlugin>();
    program = std::make_shared<OpenXRProgram>();
    extMgr = std::make_shared<OpenXRExtMgr>();

    program->createInstance();
    program->createSystem();
    program->getViewConfigurations();

    extMgr->initExtensions();

}

void YVROpenXRMgr::selectGraphicPlugin(IUnityInterfaces* interfaces)
{
    AnnounceCallingFunc();
    renderer = interfaces->Get<IUnityGraphics>()->GetRenderer();
    YDebug("Is Using Vulkan %d", renderer == kUnityGfxRendererVulkan);

    if (renderer == kUnityGfxRendererVulkan)
        graphicsPlugin = std::make_shared<GraphicsPluginVulkan>();
    else
        graphicsPlugin = std::make_shared<GraphicsPluginOpenGLES>();
}
