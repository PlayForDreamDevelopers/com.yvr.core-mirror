#include "./GraphicsPluginBase.h"

#include "GraphicsPluginVulkan.h"
#include "../../UnityHeader/IUnityGraphicsVulkan.h"

IUnityInterfaces* GraphicsPluginBase::unityInterfaces = nullptr;

void GraphicsPluginBase::setUnityInterface(IUnityInterfaces* unityInterfaces)
{
    AnnounceCallingFunc()
    GraphicsPluginBase::unityInterfaces = unityInterfaces;
    IUnityGraphicsVulkanV2* vulkanInterfaces = unityInterfaces->Get<IUnityGraphicsVulkanV2>();
    if (vulkanInterfaces != nullptr)
        GraphicsPluginVulkan::InterceptUnityVulkanInitialization(vulkanInterfaces);
}

std::vector<std::string> GraphicsPluginBase::GetInstanceExtensions()
{
    return {XR_KHR_VULKAN_ENABLE2_EXTENSION_NAME,XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME};
}
