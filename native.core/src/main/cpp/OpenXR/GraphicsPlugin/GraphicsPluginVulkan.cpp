#include "GraphicsPluginVulkan.h"
#include "../OpenXRExtMgr.h"
#include "../Common.h"

// This plugin does not link to the Vulkan loader, easier to support multiple APIs and systems that don't have Vulkan support
#define VK_NO_PROTOTYPES
#include "../../YVRUnityPlugin.h"
#include "../../UnityHeader/IUnityGraphicsVulkan.h"


static PFN_vkGetInstanceProcAddr U_vkGetInstanceProcAddr;

IUnityGraphicsVulkanV2* GraphicsPluginVulkan::vulkanUnityInterfaces = nullptr;
VkInstance GraphicsPluginVulkan::vkInstance = nullptr;

static XrInstance GetOpenXRInstanceForVulkanHook()
{
    if (!plugin.openxrMgr || !plugin.openxrMgr->program || plugin.openxrMgr->program->instance == XR_NULL_HANDLE)
    {
        YError("OpenXR instance is not ready for Vulkan hook");
        return XR_NULL_HANDLE;
    }

    return plugin.openxrMgr->program->instance;
}

static bool LoadOpenXRVulkanFunction(XrInstance instance, const char* functionName, PFN_xrVoidFunction* function)
{
    if (*function) return true;

    XrResult result = xrGetInstanceProcAddr(instance, functionName, function);
    if (XR_FAILED(result) || !*function)
    {
        YError("Failed to load %s for Vulkan hook: %s", functionName, to_string(result));
        return false;
    }

    return true;
}

static VKAPI_ATTR VkResult VKAPI_CALL Hook_vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                                                          const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
    AnnounceCallingFunc();

    XrInstance openxrInstance = GetOpenXRInstanceForVulkanHook();
    if (openxrInstance == XR_NULL_HANDLE || !U_vkGetInstanceProcAddr ||
        !LoadOpenXRVulkanFunction(openxrInstance, "xrGetVulkanGraphicsRequirements2KHR",
                                  reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsRequirements2KHR)) ||
        !LoadOpenXRVulkanFunction(openxrInstance, "xrGetVulkanGraphicsDevice2KHR",
                                  reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsDevice2KHR)) ||
        !LoadOpenXRVulkanFunction(openxrInstance, "xrCreateVulkanDeviceKHR",
                                  reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateVulkanDeviceKHR)))
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkResult result{};

    XrGraphicsRequirementsVulkan2KHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR};
    OpenXRAPI(xrGetVulkanGraphicsRequirements2KHR(plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId, &graphicsRequirements));
    YInfo("Min required version %s", GetXrVersionString(graphicsRequirements.minApiVersionSupported).c_str());

    XrVulkanGraphicsDeviceGetInfoKHR deviceGetInfo{XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR};
    deviceGetInfo.systemId = plugin.openxrMgr->program->systemId;
    deviceGetInfo.vulkanInstance = GraphicsPluginVulkan::vkInstance;
    OpenXRAPI(xrGetVulkanGraphicsDevice2KHR(plugin.openxrMgr->program->instance, &deviceGetInfo, &physicalDevice));

    XrVulkanDeviceCreateInfoKHR deviceCreateInfo{XR_TYPE_VULKAN_DEVICE_CREATE_INFO_KHR};
    deviceCreateInfo.systemId = plugin.openxrMgr->program->systemId;
    deviceCreateInfo.pfnGetInstanceProcAddr = U_vkGetInstanceProcAddr;
    deviceCreateInfo.vulkanCreateInfo = pCreateInfo;
    deviceCreateInfo.vulkanPhysicalDevice = physicalDevice;
    deviceCreateInfo.vulkanAllocator = pAllocator;
    OpenXRAPI(xrCreateVulkanDeviceKHR(plugin.openxrMgr->program->instance, &deviceCreateInfo, pDevice, &result));

    return result;
}

static VKAPI_ATTR VkResult VKAPI_CALL Hook_vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                                            VkInstance* pInstance)
{
    AnnounceCallingFunc();

    XrInstance openxrInstance = GetOpenXRInstanceForVulkanHook();
    if (openxrInstance == XR_NULL_HANDLE || !U_vkGetInstanceProcAddr ||
        !LoadOpenXRVulkanFunction(openxrInstance, "xrCreateVulkanInstanceKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateVulkanInstanceKHR)))
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkResult result{};

    XrVulkanInstanceCreateInfoKHR createInfo{XR_TYPE_VULKAN_INSTANCE_CREATE_INFO_KHR};
    createInfo.systemId = plugin.openxrMgr->program->systemId;
    createInfo.pfnGetInstanceProcAddr = U_vkGetInstanceProcAddr;
    createInfo.vulkanCreateInfo = pCreateInfo;
    createInfo.vulkanAllocator = pAllocator;
    OpenXRAPI(xrCreateVulkanInstanceKHR(plugin.openxrMgr->program->instance,&createInfo,pInstance,&result));

    GraphicsPluginVulkan::vkInstance = *pInstance;
    return result;
}

static VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL Hook_vkGetInstanceProcAddr(VkInstance device, const char* funcName)
{
    AnnounceCallingFunc();
    if (!funcName) return nullptr;

#define INTERCEPT(fn) if (strcmp(funcName, #fn) == 0) return (PFN_vkVoidFunction)&Hook_##fn
    INTERCEPT(vkCreateInstance);
    INTERCEPT(vkCreateDevice);
#undef INTERCEPT

    return U_vkGetInstanceProcAddr ? U_vkGetInstanceProcAddr(device, funcName) : nullptr;
}

static PFN_vkGetInstanceProcAddr UNITY_INTERFACE_API InterceptVulkanInitialization(PFN_vkGetInstanceProcAddr getInstanceProcAddr, void*)
{
    AnnounceCallingFunc();
    U_vkGetInstanceProcAddr = getInstanceProcAddr;
    return Hook_vkGetInstanceProcAddr;
}

void GraphicsPluginVulkan::InterceptUnityVulkanInitialization(IUnityGraphicsVulkanV2* unityVulkanInterfaces)
{
    AnnounceCallingFunc()
    vulkanUnityInterfaces = unityVulkanInterfaces;
    vulkanUnityInterfaces->AddInterceptInitialization(InterceptVulkanInitialization, nullptr, 0);
}

GraphicsPluginVulkan::GraphicsPluginVulkan() = default;

GraphicsPluginVulkan::~GraphicsPluginVulkan() = default;

void GraphicsPluginVulkan::InitializeDevice(XrInstance instance, XrSystemId systemId)
{
    AnnounceCallingFunc();

    if (!LoadOpenXRVulkanFunction(instance, "xrGetVulkanGraphicsRequirements2KHR",
                                  reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsRequirements2KHR)) ||
        !LoadOpenXRVulkanFunction(instance, "xrGetVulkanGraphicsDevice2KHR",
                                  reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsDevice2KHR)))
    {
        YError("GraphicsPluginVulkan::InitializeDevice failed to load required OpenXR Vulkan functions");
        return;
    }

    XrGraphicsRequirementsVulkan2KHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR};
    const XrResult requirementsResult = xrGetVulkanGraphicsRequirements2KHR(instance, systemId, &graphicsRequirements);
    OpenXRResult(requirementsResult, "xrGetVulkanGraphicsRequirements2KHR");
    if (XR_FAILED(requirementsResult))
    {
        YError("GraphicsPluginVulkan::InitializeDevice graphics requirements failed result=%s(%d)",
               to_string(requirementsResult), requirementsResult);
        return;
    }
    YInfo("GraphicsPluginVulkan::InitializeDevice graphics requirements min=%s max=%s",
          GetXrVersionString(graphicsRequirements.minApiVersionSupported).c_str(),
          GetXrVersionString(graphicsRequirements.maxApiVersionSupported).c_str());

    const UnityVulkanInstance& vulkanInstance = vulkanUnityInterfaces->Instance();

    XrVulkanGraphicsDeviceGetInfoKHR deviceGetInfo{XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR};
    deviceGetInfo.systemId = systemId;
    deviceGetInfo.vulkanInstance = vulkanInstance.instance;
    const XrResult deviceResult = xrGetVulkanGraphicsDevice2KHR(instance, &deviceGetInfo, &device);
    OpenXRResult(deviceResult, "xrGetVulkanGraphicsDevice2KHR");
    if (XR_FAILED(deviceResult))
    {
        YError("GraphicsPluginVulkan::InitializeDevice graphics device query failed result=%s(%d)",
               to_string(deviceResult), deviceResult);
        return;
    }
    YInfo("GraphicsPluginVulkan::InitializeDevice unityPhysicalDevice=%p runtimePhysicalDevice=%p device=%p",
          vulkanInstance.physicalDevice, device, vulkanInstance.device);

    graphicsBinding.instance = vulkanInstance.instance;
    graphicsBinding.physicalDevice = vulkanInstance.physicalDevice;
    graphicsBinding.device = vulkanInstance.device;
    graphicsBinding.queueFamilyIndex = vulkanInstance.queueFamilyIndex;
    graphicsBinding.queueIndex = 0;

    AnnounceCalledFunc();
}

const XrBaseInStructure* GraphicsPluginVulkan::GetGraphicsBinding() const { return reinterpret_cast<const XrBaseInStructure*>(&graphicsBinding); }

int64_t GraphicsPluginVulkan::getTextureFormat(bool isSRGB) { return isSRGB ? VK_FORMAT_R8G8B8A8_SRGB : VK_FORMAT_R8G8B8A8_UNORM; }
int64_t GraphicsPluginVulkan::getDepthFormat() { return VK_FORMAT_D24_UNORM_S8_UINT; }

int64_t GraphicsPluginVulkan::getMotionVectorTextureFormat() { return VK_FORMAT_R16G16B16A16_SFLOAT; }
int64_t GraphicsPluginVulkan::getMotionVectorDepthFormat() { return VK_FORMAT_D24_UNORM_S8_UINT; }

XrSwapchainImageBaseHeader* GraphicsPluginVulkan::allocateSwapchainImages(uint32_t capacity)
{
    XrSwapchainImageVulkanKHR* swapchainImageBuffer = new XrSwapchainImageVulkanKHR[capacity];

    for (int i = 0; i != capacity; ++i)
    {
        swapchainImageBuffer[i].type = XR_TYPE_SWAPCHAIN_IMAGE_VULKAN2_KHR;
        swapchainImageBuffer[i].next = NULL;
    }

    return reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImageBuffer);
}
void GraphicsPluginVulkan::deallocateSwapchainImages(XrSwapchainImageBaseHeader* images) { delete[] images; }

uintptr_t GraphicsPluginVulkan::getImageHandle(XrSwapchainImageBaseHeader* swapChain, int index)
{
    return (std::uintptr_t)reinterpret_cast<const XrSwapchainImageVulkanKHR*>(swapChain)[index].image;
}
