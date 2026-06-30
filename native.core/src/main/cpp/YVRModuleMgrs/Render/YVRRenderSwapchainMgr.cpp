#include "YVRRenderSwapchainMgr.h"

#include "../../YVRUnityPlugin.h"

YVRRenderSwapchainMgr::YVRRenderSwapchainMgr() {}

YVRRenderSwapchainMgr::~YVRRenderSwapchainMgr() {}

void YVRRenderSwapchainMgr::createSwapChain(int layerId, const XrSwapchainCreateInfo* createInfo, XrSwapchain* swapChain, bool isSurfaceSwapChain, jobject* surface)
{
    if (!isSurfaceSwapChain)
    {
        OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, createInfo, swapChain));
    }
    else
    {
        OpenXRAPI(xrCreateSwapchainAndroidSurfaceKhr(plugin.openxrMgr->program->session, createInfo, swapChain, surface));
    }
    id2SwapChainMap.insert({layerId, swapChain});
}

XrSwapchain* YVRRenderSwapchainMgr::getSwapChain(int layerId)
{
    std::map<int, XrSwapchain*>::iterator iterator = id2SwapChainMap.find(layerId);
    if (iterator == id2SwapChainMap.end())
    {
        YError("ERROR: There is no swapchain with id %d .", layerId);
        return nullptr;
    }

    return iterator->second;
}

void YVRRenderSwapchainMgr::destroySwapChain(int layerId, XrSwapchain* swapChainHandle)
{ 
    OpenXRAPI(xrDestroySwapchain(*swapChainHandle));
    XrSwapchain* swapChain = getSwapChain(layerId);
    if (swapChain == nullptr) return;
    id2SwapChainMap.erase(id2SwapChainMap.find(layerId));
}