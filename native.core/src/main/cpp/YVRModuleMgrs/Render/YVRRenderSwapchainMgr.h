#pragma once

#include <map>
#include "openxr/openxr.h"
#include <jni.h>

class YVRRenderSwapchainMgr
{
public:
    YVRRenderSwapchainMgr();
    ~YVRRenderSwapchainMgr();

    void createSwapChain(int layerId, const XrSwapchainCreateInfo* createInfo, XrSwapchain* swapChain, bool isSurfaceSwapChain, jobject* surface);
    XrSwapchain* getSwapChain(int layerId);
    void destroySwapChain(int layerId, XrSwapchain* swapChainHandle);

private:
    std::map<int, XrSwapchain*> id2SwapChainMap{};
};
