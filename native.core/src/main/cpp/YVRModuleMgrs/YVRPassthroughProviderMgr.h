#pragma once

#include "../OpenXR/OpenXRSwapchain.h"
#include "./Passthrough/PassthroughMode.h"
#include "./Passthrough/PassthroughStyle.h"

class YVRPassthroughProviderMgr
{
  public:
    YVRPassthroughProviderMgr();
    ~YVRPassthroughProviderMgr();

    void createPassthroughProvider();
    void destroyPassthroughProvider();
    void startPassthroughProvider();
    void stopPassthroughProvider();
    void createPassthroughFB();
    void destroyPassthroughFB();
    void passthroughStartFB();
    void passthroughStopFB();
    void createLargePassthroughStream();
    void destroyLargePassthroughStream();
    void largePassthrouthSteamSwapChain();
    void allocLargeSwapChain();

    bool isPassthroughInitialized();
    void createPassthroughLayer();
    void destroyPassthroughLayer();
    XrResult passthroughLayerSetStyle(PassthroughStyle style);
    XrResult createPassthroughColorLut(XrPassthroughColorLutChannelsMETA channels, uint32_t resolution, XrPassthroughColorLutDataMETA data,
                                   XrPassthroughColorLutMETA* colorLut);
    XrResult destroyPassthroughColorLut(XrPassthroughColorLutMETA lutHandle);
    XrResult updatePassthroughColorLut(XrPassthroughColorLutMETA lutHandle, XrPassthroughColorLutDataMETA data);

    void swapChain();
    XrQuaternionf getPassthroughImageDiffRotation(int eyeIndex);
    int getSwapchainImageIndex();
    int getLargeSwapchainImageIndex();
    void allocSwapChain();
    void destroySwapChain();
    bool isImageValid();
    bool isLargeImageValid();
    XrPassthroughLayerFB passthroughLayer = XR_NULL_HANDLE;
    XrPassthroughFB passthrough = XR_NULL_HANDLE;
    XrPassthroughProviderYVR largeXRPassthroughProvider = XR_NULL_HANDLE;

  private:
    XrSwapchainImageBaseHeader* swapchainImages{nullptr};
    XrSwapchainImageBaseHeader* largeSwapchainImages{nullptr};
    bool passThroughProviderCreated = false;
    bool passThroughProviderStarted = false;
    bool swapchainImagesCreated = false;
    bool swapchainAllocated = false;
    bool largePassThroughProviderStarted = false;
    bool largeSwapchainImagesCreated = false;
    bool largeSwapchainAllocated = false;
    int passThroughLayerId = -1;
    uint32_t length{0};
    uint32_t largeLength{0};
    XrResult imageAccquireResult{XR_ERROR_RUNTIME_FAILURE};
    XrResult largeImageAccquireResult{XR_ERROR_RUNTIME_FAILURE};
    XrPassthroughProviderYVR xrPassthroughProvider = XR_NULL_HANDLE;
    XrPassthroughSwapchainYVR xrPassthroughSwapchain = XR_NULL_HANDLE;

    XrPassthroughSwapchainYVR largeXRPassthroughSwapchain = XR_NULL_HANDLE;

    XrPassthroughImageYVR xrPassthroughImage{XR_TYPE_PASSTHROUGH_IMAGE_YVR};
    XrPassthroughImageYVR largeXRPassthroughImage{XR_TYPE_PASSTHROUGH_IMAGE_YVR};
    XrPassthroughColorLutMETA colorLutHandle = XR_NULL_HANDLE;
};
