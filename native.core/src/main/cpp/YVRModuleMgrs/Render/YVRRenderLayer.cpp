#include "YVRRenderLayer.h"
#include <algorithm>

#include "../../YVRUnityPlugin.h"

const int YVRRenderLayer::waitingFrames2Release = 4;

YVRRenderLayer::~YVRRenderLayer()
{
    disable();
    delete xrLayer;
}
YVRRenderLayer::YVRRenderLayer(int id): id(id) {}

YVRRenderLayer::YVRRenderLayer(int id, LayerCreateInfo createInfo)
    : id(id),
      depth(createInfo.depth),
      separateSwapChain(createInfo.separateSwapChain),
      bufferCount(createInfo.swapChainBufferCount),
      primarySwapchainIndex(-1),
      IsFlagLocked(false),
      remainingFrame2Release(-1),
      deleteLayerAfterReleaseSwapchain(false),
      isSurfaceSwapChain(createInfo.isSurfaceSwapChain),
      batchedLayerId(createInfo.batchedLayerId),
      isStatic(bufferCount != SWAP_BUFFER_NUM),
      acquireOnce(false)
{
    int arraySize = createInfo.textureType == xrTextureType::kTypeTexture ? 1 : 2;
    const int format = plugin.openxrMgr->graphicsPlugin->getTextureFormat(
        plugin.renderMgr->configsMgr->getColorSpace() == XR_COLOR_SPACE_SRGB_YVR);

    const float separateRenderScale = plugin.renderMgr->configsMgr->isUsingQuadViews() ?
                                          (depth == 0) ? plugin.renderMgr->configsMgr->getOuterPassResolutionScale() :
                                              (depth == plugin.renderMgr->configsMgr->getExtraRenderPassDepth() && plugin.renderMgr->configsMgr->
                                               hasExtraRenderPass()) ? plugin.renderMgr->configsMgr->getOuterExtraPassResolutionScale() :
                                              1.0f : 1.0f;
    primarySwapchain.width = static_cast<int>(static_cast<float>(createInfo.width) * separateRenderScale);
    primarySwapchain.height = static_cast<int>(static_cast<float>(createInfo.height) * separateRenderScale);
    primarySwapchain.swapchainAllocated = false;

    xrSwapchainCreateInfo = XrSwapchainCreateInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO};
    xrSwapchainCreateInfo.arraySize = arraySize;
    xrSwapchainCreateInfo.format = format;
    xrSwapchainCreateInfo.width = static_cast<int>(static_cast<float>(createInfo.width) * separateRenderScale);
    xrSwapchainCreateInfo.height = static_cast<int>(static_cast<float>(createInfo.height) * separateRenderScale);
    xrSwapchainCreateInfo.mipCount = 1;
    xrSwapchainCreateInfo.faceCount = 1;
    xrSwapchainCreateInfo.sampleCount = 1;
    xrSwapchainCreateInfo.createFlags = isStatic ? XR_SWAPCHAIN_CREATE_STATIC_IMAGE_BIT : 0;
    xrSwapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
    if(createInfo.protectedContent == XR_SWAPCHAIN_CREATE_PROTECTED_CONTENT_BIT)
        setDRMSwapChain();

    if (!isSurfaceSwapChain && batchedLayerId == -1)
    {
        primarySwapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
    }
}

void YVRRenderLayer::setDRMSwapChain()
{
    xrSwapchainCreateInfo.createFlags = XR_SWAPCHAIN_CREATE_PROTECTED_CONTENT_BIT;
    xrSwapchainCreateInfo.usageFlags = 0;
    xrSwapchainCreateInfo.format = 0;
    xrSwapchainCreateInfo.sampleCount = 0;
    xrSwapchainCreateInfo.faceCount = 0;
    xrSwapchainCreateInfo.arraySize = 0;
    xrSwapchainCreateInfo.mipCount = 0;
}

void YVRRenderLayer::tick()
{
    //static swapchain only acquire and release swapchainimage once in lifecycle
    if (swapchainImageAcquired)
    {
        const XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
        OpenXRAPI(xrReleaseSwapchainImage(primarySwapchain.handle, &releaseInfo));
        swapchainImageAcquired = false;
    }

    if (remainingFrame2Release == -1) return;
    remainingFrame2Release--;
    if (remainingFrame2Release <= 0)
    {
        destroySwapChain();
        remainingFrame2Release = -1;
        if (deleteLayerAfterReleaseSwapchain)
        {
            plugin.renderMgr->renderLayersMgr->addRenderLayerIdToBeDeletedLayerVec(id);
            delete this;
        }
    }
}

void YVRRenderLayer::enable()
{
    if (remainingFrame2Release != -1) // If swap chain is waiting to be released, directly cancel the waiting;
    {
        remainingFrame2Release = -1;
        return;
    }

    allocSwapChain();
}

void YVRRenderLayer::visible()
{
    if (!isSurfaceSwapChain && !isStatic)
    {
        enable();
    }
}

void YVRRenderLayer::resume()
{
    if (!isSurfaceSwapChain && !isStatic)
    {
        enable();
    }
}

void YVRRenderLayer::pause()
{
    if (!isSurfaceSwapChain && !isStatic)
    {
        disable(true);
    }
}

void YVRRenderLayer::invisible()
{
    if (!isSurfaceSwapChain && !isStatic)
    {
        disable(true);
    }
}

void YVRRenderLayer::disable(bool immediately)
{
    if (immediately) destroySwapChain();

    remainingFrame2Release = immediately ? -1 : waitingFrames2Release;
}

void YVRRenderLayer::allocSwapChain()
{
    if (primarySwapchain.swapchainAllocated) return;
    if (batchedLayerId == -1)
    {
        plugin.renderMgr->renderSwapChainMgr->createSwapChain(id, &xrSwapchainCreateInfo, &primarySwapchain.handle, isSurfaceSwapChain, &surface);
        if (!isSurfaceSwapChain)
        {
            OpenXRAPI(xrEnumerateSwapchainImages(primarySwapchain.handle, bufferCount, nullptr, primarySwapchainImages));
        }
    }
    else
    {
        primarySwapchain.handle = *plugin.renderMgr->renderSwapChainMgr->getSwapChain(id);
    }

    if (isSurfaceSwapChain)
    {
        compositionLayerImageLayoutFB.flags = XR_COMPOSITION_LAYER_IMAGE_LAYOUT_VERTICAL_FLIP_BIT_FB;
        writeLayerNext();
    }

    primarySwapchain.swapchainAllocated = true;

    // Acquire image after allocated
    swapChain();
}

void YVRRenderLayer::destroySwapChain()
{
    if (!primarySwapchain.swapchainAllocated) return;

    if (batchedLayerId == -1)
    {
        plugin.renderMgr->renderSwapChainMgr->destroySwapChain(id, &primarySwapchain.handle);
    }

    primarySwapchain.swapchainAllocated = false;
    swapchainImageAcquired = false;
}

void YVRRenderLayer::swapChain()
{
    if (!primarySwapchain.swapchainAllocated || swapchainImageAcquired
        //surfaceSwapchain call acquire is invalid
        || isSurfaceSwapChain
        //batachedLayerId != -1 means this layer share the same swapchain with layer who layerid is batchedLayerId
        || batchedLayerId != -1
        //static image can only aquire once
        || (acquireOnce && isStatic))
        return;

    constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
    OpenXRAPI(xrAcquireSwapchainImage(primarySwapchain.handle, &acquired, &primarySwapchainIndex));

    XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
    waitInfo.timeout = XR_INFINITE_DURATION;
    OpenXRAPI(xrWaitSwapchainImage(primarySwapchain.handle, &waitInfo));

    swapchainImageAcquired = true;

    if (isStatic)
    {
        acquireOnce = true;
    }
}

int YVRRenderLayer::getBufferIndex() const { return static_cast<int>(primarySwapchainIndex); }

void YVRRenderLayer::lockLayerFlag(int flags)
{
    xrLayer->layerFlags = flags;
    IsFlagLocked = true;
}

void YVRRenderLayer::unLockLayerFlag() { IsFlagLocked = false; }

bool YVRRenderLayer::getFlagLocked() { return IsFlagLocked; }

void YVRRenderLayer::setFlags(int flags)
{
    xrLayer->layerFlags |= flags;
}

void YVRRenderLayer::unsetFlags(int flags)
{
    xrLayer->layerFlags &= ~flags;
}

std::uintptr_t YVRRenderLayer::getColorHandle(xrEyeSide eyeSide)
{
    return getColorHandle(eyeSide, primarySwapchainIndex == -1 ? 0 : static_cast<int>(primarySwapchainIndex));
}

std::uintptr_t YVRRenderLayer::getColorHandle(xrEyeSide eyeSide, int bufferIndex)
{
    if (bufferIndex == -1) return getColorHandle(eyeSide);
    return plugin.openxrMgr->graphicsPlugin->getImageHandle(primarySwapchainImages, bufferIndex);
}

void YVRRenderLayer::setPose(XrPosef pose)
{}

void YVRRenderLayer::setSize(XrExtent2Df size)
{}

void YVRRenderLayer::setImageRect(yvrRect rect)
{}

void YVRRenderLayer::setSurfaceSwapChainDimensions(int width, int height)
{
    if (isSurfaceSwapChain && batchedLayerId == -1)
    {
        XrSwapchainStateAndroidSurfaceDimensionsFB androidSurfaceDimensions{XR_TYPE_SWAPCHAIN_STATE_ANDROID_SURFACE_DIMENSIONS_FB};
        androidSurfaceDimensions.width = width;
        androidSurfaceDimensions.height = height;
        xrUpdateSwapchainFB(primarySwapchain.handle, reinterpret_cast<XrSwapchainStateBaseHeaderFB*>(&androidSurfaceDimensions));
    }
}

void YVRRenderLayer::setLayerSettings(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen)
{
    AnnounceCalledFunc();

    XrCompositionLayerSettingsFlagsFB superSampleSettingsFlags = enableSuperSample ? expensiveSuperSample
                                                                                         ? XR_COMPOSITION_LAYER_SETTINGS_QUALITY_SUPER_SAMPLING_BIT_FB
                                                                                         : XR_COMPOSITION_LAYER_SETTINGS_NORMAL_SUPER_SAMPLING_BIT_FB
                                                                     : 0x00000000;

    XrCompositionLayerSettingsFlagsFB sharpenSettingsFlags = enableSharpen ? expensiveSharpen
                                                                                 ? XR_COMPOSITION_LAYER_SETTINGS_QUALITY_SHARPENING_BIT_FB
                                                                                 : XR_COMPOSITION_LAYER_SETTINGS_NORMAL_SHARPENING_BIT_FB
                                                                 : 0x00000000;

    XrCompositionLayerSettingsFlagsFB settingsFlags = superSampleSettingsFlags | sharpenSettingsFlags;
    YDebug("setLayerSettings %d", settingsFlags);
    settings.layerFlags = settingsFlags;
    writeLayerNext();
}

void YVRRenderLayer::writeLayerNext()
{
    compositionLayerImageLayoutFB.next = nullptr;
    dpCompositionLayerYVR.next = nullptr;
    settings.next = nullptr;

    const void* nextPtr = &compositionLayerImageLayoutFB;
    if (isSupportDPComposition)
    {
        dpCompositionLayerYVR.next = &compositionLayerImageLayoutFB;
        nextPtr = &dpCompositionLayerYVR;
    }

    settings.next = nextPtr;
    xrLayer->next = &settings;
}

void YVRRenderLayer::setSupportDPComposition(bool isSupport) { this->isSupportDPComposition = isSupport; }

jobject YVRRenderLayer::GetSurface()
{
    if (isSurfaceSwapChain)
    {
        return surface;
    }
    return NULL;
}
