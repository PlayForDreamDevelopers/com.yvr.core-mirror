#include "YVRPassthroughProviderMgr.h"
#include "../Utilities/YVRMath.h"
#include "../YVRUnityPlugin.h"

namespace
{
constexpr uint32_t kMaxPassthroughSwapchainImages = 16;
}

YVRPassthroughProviderMgr::YVRPassthroughProviderMgr()
    : passThroughProviderCreated(false), passThroughProviderStarted(false), swapchainImagesCreated(false), swapchainAllocated(false)
{
}

YVRPassthroughProviderMgr::~YVRPassthroughProviderMgr() {}

void YVRPassthroughProviderMgr::createPassthroughProvider()
{
    if (!passThroughProviderCreated)
    {
        xrPassthroughProvider = XR_NULL_HANDLE;
        const XrResult result = xrCreatePassthroughProviderYVR(plugin.openxrMgr->program->session, &xrPassthroughProvider);
        OpenXRResult(result, "xrCreatePassthroughProviderYVR");
        if (XR_FAILED(result) || xrPassthroughProvider == XR_NULL_HANDLE)
        {
            YWarn("Passthrough provider unavailable result=%d", result);
            xrPassthroughProvider = XR_NULL_HANDLE;
            return;
        }
        passThroughProviderCreated = true;
    }
}

void YVRPassthroughProviderMgr::destroyPassthroughProvider()
{
    if (passThroughProviderCreated)
    {
        passThroughProviderCreated = false;
        OpenXRAPI(xrDestroyPassthroughProviderYVR(xrPassthroughProvider));
        xrPassthroughProvider = XR_NULL_HANDLE;
    }
}

void YVRPassthroughProviderMgr::startPassthroughProvider()
{
    if (!passThroughProviderStarted)
    {
        if (!passThroughProviderCreated || !swapchainImagesCreated)
        {
            YWarn("Skip starting passthrough provider because initialization did not complete");
            return;
        }

        const XrResult result = xrStartPassthroughProviderYVR(xrPassthroughProvider);
        OpenXRResult(result, "xrStartPassthroughProviderYVR");
        if (XR_FAILED(result))
        {
            YWarn("Failed to start passthrough provider result=%d", result);
            return;
        }
        passThroughProviderStarted = true;
    }
}

void YVRPassthroughProviderMgr::stopPassthroughProvider()
{
    if (passThroughProviderStarted)
    {
        passThroughProviderStarted = false;
        OpenXRAPI(xrStopPassthroughProviderYVR(xrPassthroughProvider));
    }
}

void YVRPassthroughProviderMgr::createPassthroughFB()
{
    AnnounceCallingFunc();
    XrPassthroughCreateInfoFB xrPassthroughCreateInfo{XR_TYPE_PASSTHROUGH_CREATE_INFO_FB};
    xrPassthroughCreateInfo.flags = XR_PASSTHROUGH_LAYER_DEPTH_BIT_FB;
    OpenXRAPI(xrCreatePassthroughFB(plugin.openxrMgr->program->session, &xrPassthroughCreateInfo, &passthrough));
}

void YVRPassthroughProviderMgr::destroyPassthroughFB()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrDestroyPassthroughFB(passthrough));
}

void YVRPassthroughProviderMgr::passthroughStartFB()
{
    AnnounceCallingFunc();
    if (passthrough == XR_NULL_HANDLE) createPassthroughFB();

    OpenXRAPI(xrPassthroughStartFB(passthrough));
}

void YVRPassthroughProviderMgr::passthroughStopFB()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrPassthroughPauseFB(passthrough));
}

void YVRPassthroughProviderMgr::createPassthroughLayer()
{
    XrPassthroughLayerCreateInfoFB plci = {XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB};
    plci.passthrough = passthrough;
    plci.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
    OpenXRAPI(xrCreatePassthroughLayerFB(plugin.openxrMgr->program->session, &plci, &passthroughLayer));
    passThroughLayerId = plugin.renderMgr->renderLayersMgr->addPassthroughLayer(passthroughLayer);
    AnnounceCalledFunc();
}

void YVRPassthroughProviderMgr::destroyPassthroughLayer()
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->deletePassthroughLayer(passThroughLayerId);
    OpenXRAPI(xrDestroyPassthroughLayerFB(passthroughLayer));
    passthroughLayer = XR_NULL_HANDLE;
}

XrResult YVRPassthroughProviderMgr::passthroughLayerSetStyle(PassthroughStyle passthroughStyle)
{
    YInfo("Setting passthrough layer style to %d", passthroughStyle.textureColorMapType);
    AnnounceCallingFunc();
    if (passthroughStyle.textureColorMapType != PassthroughMode::None && passthroughLayer == XR_NULL_HANDLE)
    {
        createPassthroughLayer();
        OpenXRAPI(xrPassthroughLayerResumeFB(passthroughLayer));
    }

    XrPassthroughStyleFB style{XR_TYPE_PASSTHROUGH_STYLE_FB};
    switch (passthroughStyle.textureColorMapType)
    {
        case None:
            if (passthroughLayer != XR_NULL_HANDLE)
            {
                OpenXRAPI(xrPassthroughLayerPauseFB(passthroughLayer));
                destroyPassthroughLayer();
            }
            break;
        case MonoToRgba:
        case MonoToMono:
        case BrightnessContrastSaturation:
        case InterpolatedColorLut:
            break;
        case ColorLut:
            // Apply the color LUT to the passthrough layer
            if (passthroughStyle.lutSource != XR_NULL_HANDLE)
            {
                XrPassthroughColorMapLutMETA colorLutStyle{XR_TYPE_PASSTHROUGH_COLOR_MAP_LUT_META};
                colorLutStyle.colorLut = passthroughStyle.lutSource;
                colorLutStyle.weight = passthroughStyle.lutWeight;

                style.next = &colorLutStyle;
                XrResult result = xrPassthroughLayerSetStyleFB(passthroughLayer, &style);
                if (XR_FAILED(result))
                {
                    YInfo("XrPassthrough Failed to set style with color LUT on a passthrough layer.");
                }
                return result;
            }
            else
            {
                YInfo("XrPassthrough Color LUT handle is null, cannot apply to passthrough layer.");
                return OpenXRAPI(xrPassthroughLayerSetStyleFB(passthroughLayer, &style));
            }
            break;
        default:
            break;
    }

    return XrResult::XR_SUCCESS;
}

XrResult YVRPassthroughProviderMgr::createPassthroughColorLut(XrPassthroughColorLutChannelsMETA channels, uint32_t resolution,
                                                          XrPassthroughColorLutDataMETA data, XrPassthroughColorLutMETA* colorLut)
{
    AnnounceCallingFunc();
    if (passthroughLayer == XR_NULL_HANDLE)
    {
        createPassthroughLayer();
        OpenXRAPI(xrPassthroughLayerResumeFB(passthroughLayer));
    }

    XrPassthroughColorLutCreateInfoMETA createInfo{XR_TYPE_PASSTHROUGH_COLOR_LUT_CREATE_INFO_META};
    createInfo.channels = channels;
    createInfo.resolution = resolution;
    createInfo.data.buffer = data.buffer;
    createInfo.data.bufferSize = data.bufferSize;

    XrResult result =OpenXRAPI(xrCreatePassthroughColorLutMETA(passthrough, &createInfo, colorLut));
    YInfo("xrCreatePassthroughColorLutMETA result: %d, colorLut: %p", result, *colorLut);
    if (XR_FAILED(result))
    {
        YInfo("Failed to create a passthrough color LUT.");
    }
    return result;
}

XrResult YVRPassthroughProviderMgr::updatePassthroughColorLut(XrPassthroughColorLutMETA lutHandle, XrPassthroughColorLutDataMETA data)
{
    XrPassthroughColorLutUpdateInfoMETA updateInfo{XR_TYPE_PASSTHROUGH_COLOR_LUT_UPDATE_INFO_META};
    updateInfo.data = data;
    return OpenXRAPI(xrUpdatePassthroughColorLutMETA(lutHandle, &updateInfo));
}

XrResult YVRPassthroughProviderMgr::destroyPassthroughColorLut(XrPassthroughColorLutMETA lutHandle)
{
    AnnounceCallingFunc();
    YInfo("destroyPassthroughColorLut lutHandle: %p", lutHandle);
    return OpenXRAPI(xrDestroyPassthroughColorLutMETA(lutHandle));
}

void YVRPassthroughProviderMgr::swapChain()
{
    if (swapchainImagesCreated && passThroughProviderStarted)
    {
        XrSpace appSpace = plugin.inputMgr->GetAppSpace();
        XrPassthroughAcquireInfoYVR acquiredInfo{XR_TYPE_PASSTHROUGH_ACQUIRE_INFO_YVR, nullptr, appSpace};

        // This api may caused XR_ERROR_LAYER_INVALID error while device is just lighting up.
        // And This error should be fixed by system, or the system should add a new api to indicated whether the vst stream is available or not
        imageAccquireResult = xrAcquirePassthroughImageYVR(xrPassthroughProvider, &acquiredInfo, &xrPassthroughImage);
    }
}

XrQuaternionf YVRPassthroughProviderMgr::getPassthroughImageDiffRotation(int eyeIndex)
{
    if (!plugin.renderMgr->configsMgr->passthroughProviderEnable() || !isImageValid() || eyeIndex < 0 || eyeIndex > 1) return {};

    // From VST Pose to Render
    const XrQuaternionf vstOrientation = XrQuaternionf_Inverse(xrPassthroughImage.views[eyeIndex].pose.orientation);
    const XrQuaternionf renderOrientation = eyeIndex == 0 ? plugin.hmdMgr->lEyeRenderPose.orientation : plugin.hmdMgr->rEyeRenderPose.orientation;

    XrQuaternionf diff{};
    XrQuaternionf_Multiply(diff, renderOrientation, vstOrientation);
    return diff;
}

int YVRPassthroughProviderMgr::getSwapchainImageIndex()
{
    if (!plugin.renderMgr->configsMgr->passthroughProviderEnable() || !isImageValid() || swapchainImages == nullptr) return -1;

    const int index = static_cast<int>(xrPassthroughImage.swapchainIndex);
    if (index < 0 || static_cast<uint32_t>(index) >= length) return -1;
    uintptr_t handle = plugin.openxrMgr->graphicsPlugin->getImageHandle(swapchainImages, index);
    return handle;
}

void YVRPassthroughProviderMgr::allocSwapChain()
{
    if (!passThroughProviderCreated || xrPassthroughProvider == XR_NULL_HANDLE)
    {
        YWarn("Skip passthrough swapchain allocation because provider creation failed");
        return;
    }

    if (!swapchainAllocated)
    {
        xrPassthroughSwapchain = XR_NULL_HANDLE;
        const XrResult result = xrCreatePassthroughSwapchainYVR(xrPassthroughProvider, &xrPassthroughSwapchain);
        OpenXRResult(result, "xrCreatePassthroughSwapchainYVR");
        if (XR_FAILED(result) || xrPassthroughSwapchain == XR_NULL_HANDLE)
        {
            YWarn("Failed to create passthrough swapchain result=%d", result);
            xrPassthroughSwapchain = XR_NULL_HANDLE;
            return;
        }
        swapchainAllocated = true;
    }

    if (!swapchainImagesCreated)
    {
        uint32_t imageCount = 0;
        const XrResult countResult = xrEnumeratePassthroughSwapchainImagesYVR(xrPassthroughSwapchain, 0, &imageCount, nullptr);
        OpenXRResult(countResult, "xrEnumeratePassthroughSwapchainImagesYVR(count)");
        if (XR_FAILED(countResult) || imageCount == 0 || imageCount > kMaxPassthroughSwapchainImages)
        {
            YWarn("Invalid passthrough swapchain image count result=%d count=%u", countResult, imageCount);
            destroySwapChain();
            return;
        }

        XrSwapchainImageBaseHeader* images = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(imageCount);
        const XrResult enumerateResult =
            xrEnumeratePassthroughSwapchainImagesYVR(xrPassthroughSwapchain, imageCount, nullptr, images);
        OpenXRResult(enumerateResult, "xrEnumeratePassthroughSwapchainImagesYVR(images)");
        if (XR_FAILED(enumerateResult))
        {
            plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(images);
            YWarn("Failed to enumerate passthrough swapchain images result=%d", enumerateResult);
            destroySwapChain();
            return;
        }

        swapchainImages = images;
        length = imageCount;
        swapchainImagesCreated = true;
    }
}

void YVRPassthroughProviderMgr::destroySwapChain()
{
    if (swapchainImagesCreated)
    {
        swapchainImagesCreated = false;
        plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(swapchainImages);
        swapchainImages = nullptr;
        length = 0;
        imageAccquireResult = XR_ERROR_RUNTIME_FAILURE;
    }

    if (swapchainAllocated)
    {
        swapchainAllocated = false;
        OpenXRAPI(xrDestroyPassthroughSwapchainYVR(xrPassthroughSwapchain));
        xrPassthroughSwapchain = XR_NULL_HANDLE;
    }
}

bool YVRPassthroughProviderMgr::isImageValid()
{
    return swapchainImagesCreated && passThroughProviderStarted && imageAccquireResult == XR_SUCCESS;
}

bool YVRPassthroughProviderMgr::isPassthroughInitialized()
{
    return passThroughProviderCreated && passThroughProviderStarted && swapchainAllocated && swapchainImagesCreated;
}

void YVRPassthroughProviderMgr::createLargePassthroughStream()
{
    AnnounceCallingFunc();
    if (largeXRPassthroughProvider != XR_NULL_HANDLE) return;

    const XrResult createResult = xrCreatePassthroughProviderYVR(plugin.openxrMgr->program->session, &largeXRPassthroughProvider);
    OpenXRResult(createResult, "xrCreatePassthroughProviderYVR(large)");
    if (XR_FAILED(createResult) || largeXRPassthroughProvider == XR_NULL_HANDLE)
    {
        YWarn("Large passthrough provider unavailable result=%d", createResult);
        largeXRPassthroughProvider = XR_NULL_HANDLE;
        return;
    }

    allocLargeSwapChain();
    if (!largeSwapchainImagesCreated)
    {
        destroyLargePassthroughStream();
        return;
    }

    const XrResult startResult = xrStartPassthroughProviderYVR(largeXRPassthroughProvider);
    OpenXRResult(startResult, "xrStartPassthroughProviderYVR(large)");
    if (XR_FAILED(startResult))
    {
        YWarn("Failed to start large passthrough provider result=%d", startResult);
        destroyLargePassthroughStream();
        return;
    }
    largePassThroughProviderStarted = true;
}

void YVRPassthroughProviderMgr::destroyLargePassthroughStream()
{
    if (largePassThroughProviderStarted)
    {
        largePassThroughProviderStarted = false;
        OpenXRAPI(xrStopPassthroughProviderYVR(largeXRPassthroughProvider));
    }
    if (largeSwapchainImagesCreated)
    {
        largeSwapchainImagesCreated = false;
        plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(largeSwapchainImages);
        largeSwapchainImages = nullptr;
        largeLength = 0;
        largeImageAccquireResult = XR_ERROR_RUNTIME_FAILURE;
    }
    if (largeSwapchainAllocated)
    {
        largeSwapchainAllocated = false;
        OpenXRAPI(xrDestroyPassthroughSwapchainYVR(largeXRPassthroughSwapchain));
        largeXRPassthroughSwapchain = XR_NULL_HANDLE;
    }
    if (largeXRPassthroughProvider != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrDestroyPassthroughProviderYVR(largeXRPassthroughProvider));
        largeXRPassthroughProvider = XR_NULL_HANDLE;
    }
}

void YVRPassthroughProviderMgr::allocLargeSwapChain()
{
    AnnounceCallingFunc();
    if (largeXRPassthroughProvider == XR_NULL_HANDLE) return;

    const XrResult createResult = xrCreatePassthroughSwapchainYVR2(largeXRPassthroughProvider, &largeXRPassthroughSwapchain);
    OpenXRResult(createResult, "xrCreatePassthroughSwapchainYVR2");
    if (XR_FAILED(createResult) || largeXRPassthroughSwapchain == XR_NULL_HANDLE)
    {
        YWarn("Failed to create large passthrough swapchain result=%d", createResult);
        largeXRPassthroughSwapchain = XR_NULL_HANDLE;
        return;
    }
    largeSwapchainAllocated = true;

    uint32_t imageCount = 0;
    const XrResult countResult = xrEnumeratePassthroughSwapchainImagesYVR(largeXRPassthroughSwapchain, 0, &imageCount, nullptr);
    OpenXRResult(countResult, "xrEnumeratePassthroughSwapchainImagesYVR(large count)");
    if (XR_FAILED(countResult) || imageCount == 0 || imageCount > kMaxPassthroughSwapchainImages)
    {
        YWarn("Invalid large passthrough swapchain image count result=%d count=%u", countResult, imageCount);
        return;
    }

    XrSwapchainImageBaseHeader* images = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(imageCount);
    const XrResult enumerateResult =
        xrEnumeratePassthroughSwapchainImagesYVR(largeXRPassthroughSwapchain, imageCount, nullptr, images);
    OpenXRResult(enumerateResult, "xrEnumeratePassthroughSwapchainImagesYVR(large images)");
    if (XR_FAILED(enumerateResult))
    {
        plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(images);
        YWarn("Failed to enumerate large passthrough swapchain images result=%d", enumerateResult);
        return;
    }

    largeSwapchainImages = images;
    largeLength = imageCount;
    largeSwapchainImagesCreated = true;
}

void YVRPassthroughProviderMgr::largePassthrouthSteamSwapChain()
{
    if (!largePassThroughProviderStarted || !largeSwapchainImagesCreated) return;

    XrSpace appSpace = plugin.inputMgr->GetAppSpace();
    XrPassthroughAcquireInfoYVR acquiredInfo{XR_TYPE_PASSTHROUGH_ACQUIRE_INFO_YVR, nullptr, appSpace};
    largeImageAccquireResult = xrAcquirePassthroughImageYVR(largeXRPassthroughProvider, &acquiredInfo, &largeXRPassthroughImage);
}

int YVRPassthroughProviderMgr::getLargeSwapchainImageIndex()
{
    if (!isLargeImageValid() || largeSwapchainImages == nullptr) return -1;

    const int index = static_cast<int>(largeXRPassthroughImage.swapchainIndex);
    if (index < 0 || static_cast<uint32_t>(index) >= largeLength) return -1;
    uintptr_t handle = plugin.openxrMgr->graphicsPlugin->getImageHandle(largeSwapchainImages, index);
    return handle;
}

bool YVRPassthroughProviderMgr::isLargeImageValid()
{
    return largePassThroughProviderStarted && largeSwapchainImagesCreated && largeImageAccquireResult == XR_SUCCESS;
}
