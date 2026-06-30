#pragma once

#include "./GraphicsPluginBase.h"
#include "../../UnityHeader/IUnityGraphicsVulkan.h"

class GraphicsPluginVulkan : public GraphicsPluginBase
{
public:
    GraphicsPluginVulkan();
    ~GraphicsPluginVulkan() override;
    static void InterceptUnityVulkanInitialization(IUnityGraphicsVulkanV2* unityVulkanInterfaces);
    void InitializeDevice(XrInstance instance, XrSystemId systemId) override;

    const XrBaseInStructure* GetGraphicsBinding() const override;
    int64_t getTextureFormat(bool isSRGB) override;
    int64_t getDepthFormat() override;
    int64_t getMotionVectorTextureFormat() override;
    int64_t getMotionVectorDepthFormat() override;
    XrSwapchainImageBaseHeader* allocateSwapchainImages(uint32_t capacity) override;
    void deallocateSwapchainImages(XrSwapchainImageBaseHeader* images) override;
    uintptr_t getImageHandle(XrSwapchainImageBaseHeader* swapChain, int index) override;

    static VkInstance vkInstance;

private:
    static IUnityGraphicsVulkanV2* vulkanUnityInterfaces;
    XrGraphicsBindingVulkanKHR graphicsBinding{XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR};
    std::list<std::vector<XrSwapchainImageVulkanKHR>> swapchainImageBuffers{};
    GLint contextApiMajorVersion{0};
    VkPhysicalDevice device{};
};
