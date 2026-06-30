#pragma once

#include "./YVRRenderLayer.h"

class YVRProjectionRenderLayer : public YVRRenderLayer
{
public:
    YVRProjectionRenderLayer(int id, LayerCreateInfo createInfo);
    std::uintptr_t getColorHandle(xrEyeSide eyeSide, int bufferIndex) override;
    std::uintptr_t getMotionVectorColorHandle(xrEyeSide eyeSide, int bufferIndex);
    std::uintptr_t getMotionVectorDepthHandle(xrEyeSide eyeSide, int bufferIndex);
    uint32_t getMotionVectorImageRectWidth();
    uint32_t getMotionVectorImageRectHeight();
    void swapChain() override;
    void setFoveationLevel(XrFoveationLevelFB level, XrFoveationDynamicFB dynamic);
    void setCurrentPosePosition(float x, float y, float z);
    void setCurrentPoseRotation(float x, float y, float z, float w);

    std::uintptr_t getDepthHandle(xrEyeSide eyeSide, int bufferIndex);
    void setAppSWProjectionLayerViewNext();

protected:
    void allocSwapChain() override;
    void destroySwapChain() override;
    void tick() override;
private:
    XrPosef prevPose;
    XrPosef currentPose;

    std::vector<XrCompositionLayerProjectionView> projectionLayerViews{};

    OpenXRSwapchain secondarySwapchain;
    bool secondarySwapchainImageAcquired = false;
    XrSwapchainImageBaseHeader* secondarySwapchainImages;
    uint32_t secondarySwapchainIndex;

    XrSwapchainCreateInfo depthSwapChainCreateInfo{};
    std::vector<XrCompositionLayerDepthInfoKHR> depthInfo{};
    OpenXRSwapchain depthSwapchain;
    bool depthImageAcquired = false;
    XrSwapchainImageBaseHeader* depthSwapchainImages;
    uint32_t depthSwapchainIndex;

    OpenXRSwapchain depthSecondarySwapchain;
    bool depthSecondarySwapchainImageAllocated = false;
    XrSwapchainImageBaseHeader* depthSecondarySwapchainImages;
    uint32_t depthSecondarySwapchainIndex;

    XrSwapchainCreateInfo motionVectorColorSwapChainCreateInfo{};
    XrSwapchainCreateInfo motionVectorDepthSwapChainCreateInfo{};
    std::vector<XrCompositionLayerSpaceWarpInfoFB> spaceWarpInfo{};
    XrSystemSpaceWarpPropertiesFB spaceWarpProperties;
    XrSystemProperties systemProperties;
    uint32_t recommendedMotionVectorImageRectWidth;
    uint32_t recommendedMotionVectorImageRectHeight;

    OpenXRSwapchain motionVectorPrimarySwapChain;
    bool motionVectorPrimaryImageAcquired = false;
    XrSwapchainImageBaseHeader* motionVectorPrimaryImages;
    uint32_t motionVectorPrimarySwapchainIndex;

    OpenXRSwapchain motionVectorPrimaryDepthSwapChain;
    bool motionVectorPrimaryDepthImageAcquired = false;
    XrSwapchainImageBaseHeader* motionVectorPrimaryDepthImages;
    uint32_t motionVectorPrimaryDepthSwapchainIndex;

    OpenXRSwapchain motionVectorSecondarySwapChain;
    bool motionVectorSecondaryImageAcquired = false;
    XrSwapchainImageBaseHeader* motionVectorSecondaryImages;
    uint32_t motionVectorSecondarySwapchainIndex;

    OpenXRSwapchain motionVectorSecondaryDepthSwapChain;
    bool motionVectorSecondaryDepthImageAcquired = false;
    XrSwapchainImageBaseHeader* motionVectorSecondaryDepthImages;
    uint32_t motionVectorSecondaryDepthSwapchainIndex;
};
