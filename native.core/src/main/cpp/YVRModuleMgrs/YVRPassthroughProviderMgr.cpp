#include "YVRPassthroughProviderMgr.h"
#include "../Utilities/YVRMath.h"
#include "../YVRUnityPlugin.h"

YVRPassthroughProviderMgr::YVRPassthroughProviderMgr()
    : passThroughProviderCreated(false), passThroughProviderStarted(false), swapchainImagesCreated(false), swapchainAllocated(false)
{
}

YVRPassthroughProviderMgr::~YVRPassthroughProviderMgr() {}

void YVRPassthroughProviderMgr::createPassthroughProvider()
{
    if (!passThroughProviderCreated)
    {
        passThroughProviderCreated = true;
        OpenXRAPI(xrCreatePassthroughProviderYVR(plugin.openxrMgr->program->session, &xrPassthroughProvider));
    }
}

void YVRPassthroughProviderMgr::destroyPassthroughProvider()
{
    if (passThroughProviderCreated)
    {
        passThroughProviderCreated = false;
        OpenXRAPI(xrDestroyPassthroughProviderYVR(xrPassthroughProvider));
    }
}

void YVRPassthroughProviderMgr::startPassthroughProvider()
{
    if (!passThroughProviderStarted)
    {
        passThroughProviderStarted = true;
        OpenXRAPI(xrStartPassthroughProviderYVR(xrPassthroughProvider));
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
    if (swapchainAllocated && passThroughProviderStarted)
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
    if (!plugin.renderMgr->configsMgr->passthroughProviderEnable()) return {};

    // From VST Pose to Render
    const XrQuaternionf vstOrientation = XrQuaternionf_Inverse(xrPassthroughImage.views[eyeIndex].pose.orientation);
    const XrQuaternionf renderOrientation = eyeIndex == 0 ? plugin.hmdMgr->lEyeRenderPose.orientation : plugin.hmdMgr->rEyeRenderPose.orientation;

    XrQuaternionf diff{};
    XrQuaternionf_Multiply(diff, renderOrientation, vstOrientation);
    return diff;
}

int YVRPassthroughProviderMgr::getSwapchainImageIndex()
{
    if (!plugin.renderMgr->configsMgr->passthroughProviderEnable()) return -1;

    int index = static_cast<int>(xrPassthroughImage.swapchainIndex);
    uintptr_t handle = plugin.openxrMgr->graphicsPlugin->getImageHandle(swapchainImages, index);
    return handle;
}

void YVRPassthroughProviderMgr::allocSwapChain()
{
    if (!swapchainAllocated)
    {
        swapchainAllocated = true;
        OpenXRAPI(xrCreatePassthroughSwapchainYVR(xrPassthroughProvider, &xrPassthroughSwapchain));
    }

    if (!swapchainImagesCreated)
    {
        swapchainImagesCreated = true;
        OpenXRAPI(xrEnumeratePassthroughSwapchainImagesYVR(xrPassthroughSwapchain, 0, &length, NULL));
        swapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(length);
        OpenXRAPI(xrEnumeratePassthroughSwapchainImagesYVR(xrPassthroughSwapchain, length, nullptr, swapchainImages));
    }
}

void YVRPassthroughProviderMgr::destroySwapChain()
{
    if (swapchainAllocated)
    {
        swapchainAllocated = false;
        OpenXRAPI(xrDestroyPassthroughSwapchainYVR(xrPassthroughSwapchain));
    }

    if (swapchainImagesCreated)
    {
        swapchainImagesCreated = false;
        plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(swapchainImages);
    }
}

bool YVRPassthroughProviderMgr::isImageValid() { return imageAccquireResult == XR_SUCCESS; }

void YVRPassthroughProviderMgr::createLargePassthroughStream()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrCreatePassthroughProviderYVR(plugin.openxrMgr->program->session, &largeXRPassthroughProvider));
    YInfo("xrCreatePassthroughProviderYVR done");
    plugin.passThroughProviderMgr->allocLargeSwapChain();
    YInfo("allocLargeSwapChain done");
    OpenXRAPI(xrStartPassthroughProviderYVR(largeXRPassthroughProvider));
    YInfo("xrStartPassthroughProviderYVR done");
}

void YVRPassthroughProviderMgr::destroyLargePassthroughStream()
{
    OpenXRAPI(xrStopPassthroughProviderYVR(largeXRPassthroughProvider));
    OpenXRAPI(xrDestroyPassthroughSwapchainYVR(largeXRPassthroughSwapchain));
    OpenXRAPI(xrDestroyPassthroughProviderYVR(largeXRPassthroughProvider));
    plugin.openxrMgr->graphicsPlugin->deallocateSwapchainImages(largeSwapchainImages);
}

void YVRPassthroughProviderMgr::allocLargeSwapChain()
{
    AnnounceCallingFunc();
    YInfo("xrCreatePassthroughSwapchainYVR2:%p",xrCreatePassthroughSwapchainYVR2);
    OpenXRAPI(xrCreatePassthroughSwapchainYVR2(largeXRPassthroughProvider, &largeXRPassthroughSwapchain));

    OpenXRAPI(xrEnumeratePassthroughSwapchainImagesYVR(largeXRPassthroughSwapchain, 0, &length, NULL));
    largeSwapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(length);
    OpenXRAPI(xrEnumeratePassthroughSwapchainImagesYVR(largeXRPassthroughSwapchain, length, nullptr, largeSwapchainImages));
}

void YVRPassthroughProviderMgr::largePassthrouthSteamSwapChain()
{
    XrSpace appSpace = plugin.inputMgr->GetAppSpace();
    XrPassthroughAcquireInfoYVR acquiredInfo{XR_TYPE_PASSTHROUGH_ACQUIRE_INFO_YVR, nullptr, appSpace};
    largeImageAccquireResult = xrAcquirePassthroughImageYVR(largeXRPassthroughProvider, &acquiredInfo, &largeXRPassthroughImage);
}

int YVRPassthroughProviderMgr::getLargeSwapchainImageIndex()
{
    if (largeXRPassthroughProvider == XR_NULL_HANDLE) return -1;

    int index = static_cast<int>(largeXRPassthroughImage.swapchainIndex);
    uintptr_t handle = plugin.openxrMgr->graphicsPlugin->getImageHandle(largeSwapchainImages, index);
    return handle;
}

bool YVRPassthroughProviderMgr::isLargeImageValid() { return largeImageAccquireResult == XR_SUCCESS; }
