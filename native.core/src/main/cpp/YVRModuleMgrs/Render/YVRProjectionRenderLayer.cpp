#include "YVRProjectionRenderLayer.h"
#include <openxr/openxr.h>
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRMath.h"

LayerCreateInfo info;

YVRProjectionRenderLayer::YVRProjectionRenderLayer(int id, LayerCreateInfo createInfo)
    : YVRRenderLayer(id, createInfo),
      secondarySwapchain(),
      secondarySwapchainIndex(-1),
      motionVectorPrimarySwapChain(),
      motionVectorPrimaryDepthSwapChain(),
      motionVectorSecondarySwapChain(),
      motionVectorSecondaryDepthSwapChain()
{
    projectionLayerViews.resize(plugin.renderMgr->configsMgr->getViewsCount());
    depthInfo.resize(2);
    spaceWarpInfo.resize(2);
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerProjection{XR_TYPE_COMPOSITION_LAYER_PROJECTION});

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        spaceWarpProperties = {XR_TYPE_SYSTEM_SPACE_WARP_PROPERTIES_FB};
        systemProperties = {XR_TYPE_SYSTEM_PROPERTIES};
        systemProperties.next = &spaceWarpProperties;
        OpenXRAPI(xrGetSystemProperties(plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId, &systemProperties));

        recommendedMotionVectorImageRectWidth = spaceWarpProperties.recommendedMotionVectorImageRectWidth;
        recommendedMotionVectorImageRectHeight = spaceWarpProperties.recommendedMotionVectorImageRectHeight;

        motionVectorPrimarySwapChain.width = recommendedMotionVectorImageRectWidth;
        motionVectorPrimarySwapChain.height = recommendedMotionVectorImageRectHeight;
        motionVectorPrimarySwapChain.swapchainAllocated = false;

        motionVectorColorSwapChainCreateInfo = XrSwapchainCreateInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO};
        motionVectorColorSwapChainCreateInfo.arraySize = createInfo.textureType == xrTextureType::kTypeTexture ? 1 : 2;
        motionVectorColorSwapChainCreateInfo.format = plugin.openxrMgr->graphicsPlugin->getMotionVectorTextureFormat();
        motionVectorColorSwapChainCreateInfo.width = motionVectorPrimarySwapChain.width;
        motionVectorColorSwapChainCreateInfo.height = motionVectorPrimarySwapChain.height;
        motionVectorColorSwapChainCreateInfo.mipCount = 1;
        motionVectorColorSwapChainCreateInfo.faceCount = 1;
        motionVectorColorSwapChainCreateInfo.sampleCount = 1;
        motionVectorColorSwapChainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

        motionVectorPrimaryImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);

        motionVectorPrimaryDepthSwapChain.width = recommendedMotionVectorImageRectWidth;
        motionVectorPrimaryDepthSwapChain.height = recommendedMotionVectorImageRectHeight;
        motionVectorPrimaryDepthSwapChain.swapchainAllocated = false;

        motionVectorDepthSwapChainCreateInfo = XrSwapchainCreateInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO};
        motionVectorDepthSwapChainCreateInfo.arraySize = createInfo.textureType == xrTextureType::kTypeTexture ? 1 : 2;
        motionVectorDepthSwapChainCreateInfo.format = plugin.openxrMgr->graphicsPlugin->getMotionVectorDepthFormat();
        motionVectorDepthSwapChainCreateInfo.width = motionVectorPrimarySwapChain.width;
        motionVectorDepthSwapChainCreateInfo.height = motionVectorPrimarySwapChain.height;
        motionVectorDepthSwapChainCreateInfo.mipCount = 1;
        motionVectorDepthSwapChainCreateInfo.faceCount = 1;
        motionVectorDepthSwapChainCreateInfo.sampleCount = 1;
        motionVectorDepthSwapChainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        motionVectorPrimaryDepthImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);

        if (separateSwapChain)
        {
            motionVectorSecondarySwapChain.width = recommendedMotionVectorImageRectWidth;
            motionVectorSecondarySwapChain.height = recommendedMotionVectorImageRectHeight;
            motionVectorSecondarySwapChain.swapchainAllocated = false;

            motionVectorSecondaryDepthSwapChain.width = recommendedMotionVectorImageRectWidth;
            motionVectorSecondaryDepthSwapChain.height = recommendedMotionVectorImageRectHeight;
            motionVectorSecondaryDepthSwapChain.swapchainAllocated = false;

            motionVectorSecondaryImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
            motionVectorSecondaryDepthImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
        }
    }

    info = createInfo;

    if (separateSwapChain)
    {
        const float separateRenderScale = plugin.renderMgr->configsMgr->isUsingQuadViews() ?
                                              (depth == 0 ? plugin.renderMgr->configsMgr->getInnerPassResolutionScale() :
                                                   plugin.renderMgr->configsMgr->getInnerExtraPassResolutionScale())
                                              : 1.0f;
        const auto secondRes = plugin.renderMgr->configsMgr->getEyeBufferResolution(1);
        secondarySwapchain.width = static_cast<int>(secondRes.x * separateRenderScale);
        secondarySwapchain.height = static_cast<int>(secondRes.y * separateRenderScale);
        secondarySwapchain.swapchainAllocated = false;
        secondarySwapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
    }

    if (plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
    {
        depthSwapchain.width = static_cast<int>(plugin.renderMgr->configsMgr->getEyeBufferResolution().x);
        depthSwapchain.height = static_cast<int>(plugin.renderMgr->configsMgr->getEyeBufferResolution().y);
        depthSwapchain.swapchainAllocated = false;
        depthSwapChainCreateInfo.type = XR_TYPE_SWAPCHAIN_CREATE_INFO;
        depthSwapChainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        depthSwapChainCreateInfo.format = plugin.openxrMgr->graphicsPlugin->getDepthFormat();
        depthSwapChainCreateInfo.sampleCount = 1;
        depthSwapChainCreateInfo.width = static_cast<uint32_t>(plugin.renderMgr->configsMgr->getEyeBufferResolution().x);
        depthSwapChainCreateInfo.height = static_cast<uint32_t>(plugin.renderMgr->configsMgr->getEyeBufferResolution().y);
        depthSwapChainCreateInfo.faceCount = 1;
        depthSwapChainCreateInfo.arraySize = separateSwapChain ? 1 : 2;
        depthSwapChainCreateInfo.mipCount = 1;

        depthSwapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
        if (separateSwapChain) depthSecondarySwapchainImages = plugin.openxrMgr->graphicsPlugin->allocateSwapchainImages(bufferCount);
    }
}

void YVRProjectionRenderLayer::tick()
{
    YVRRenderLayer::tick();

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        if (motionVectorPrimaryImageAcquired)
        {
            constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            OpenXRAPI(xrReleaseSwapchainImage(motionVectorPrimarySwapChain.handle, &releaseInfo));
            motionVectorPrimaryImageAcquired = false;
        }

        if (motionVectorPrimaryDepthImageAcquired)
        {
            constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            OpenXRAPI(xrReleaseSwapchainImage(motionVectorPrimaryDepthSwapChain.handle, &releaseInfo));
            motionVectorPrimaryDepthImageAcquired = false;
        }

        if (motionVectorSecondaryImageAcquired)
        {
            constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            OpenXRAPI(xrReleaseSwapchainImage(motionVectorSecondarySwapChain.handle, &releaseInfo));
            motionVectorSecondaryImageAcquired = false;
        }

        if (motionVectorSecondaryDepthImageAcquired)
        {
            constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            OpenXRAPI(xrReleaseSwapchainImage(motionVectorSecondaryDepthSwapChain.handle, &releaseInfo));
            motionVectorSecondaryDepthImageAcquired = false;
        }
    }

    if (secondarySwapchainImageAcquired && separateSwapChain)
    {
        constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
        OpenXRAPI(xrReleaseSwapchainImage(secondarySwapchain.handle, &releaseInfo));
        secondarySwapchainImageAcquired = false;
    }

    if (depthImageAcquired && plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
    {
        constexpr XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
        OpenXRAPI(xrReleaseSwapchainImage(depthSwapchain.handle, &releaseInfo));
        if (separateSwapChain)
            OpenXRAPI(xrReleaseSwapchainImage(depthSecondarySwapchain.handle, &releaseInfo));
        depthImageAcquired = false;
    }

    const std::vector<XrView> views = plugin.hmdMgr->renderViews;

    for (int i = 0; i != plugin.renderMgr->configsMgr->getViewsCount(); ++i)
    {
        projectionLayerViews[i].pose = i % 2 == 0 ? plugin.hmdMgr->lEyeRenderPose : plugin.hmdMgr->rEyeRenderPose;
        projectionLayerViews[i].fov = views[i].fov;
        if (plugin.renderMgr->configsMgr->isAppSWEnable())
        {
            XrPosef inversePrevPose = XrPosef_Inverse(prevPose);
            spaceWarpInfo[i].appSpaceDeltaPose = XrPosef_Multiply(inversePrevPose, currentPose);
            prevPose.position = currentPose.position;
            prevPose.orientation = currentPose.orientation;
        }

        const bool isPrimarySwapChain = plugin.renderMgr->configsMgr->isUsingQuadViews() ? i / 2 == 0 : (separateSwapChain ? i % 2 == 0 : true);

        int32_t primarySwapchainWidth = int32_t(primarySwapchain.width * plugin.renderMgr->configsMgr->getRenderViewport().x);
        int32_t secondarySwapchainWidth = int32_t(secondarySwapchain.width * plugin.renderMgr->configsMgr->getRenderViewport().x);
        int32_t primarySwapchainHeight = int32_t(primarySwapchain.height * plugin.renderMgr->configsMgr->getRenderViewport().y);
        int32_t secondarySwapchainHeight = int32_t(secondarySwapchain.height * plugin.renderMgr->configsMgr->getRenderViewport().y);

        projectionLayerViews[i].subImage.imageRect.offset = {0, 0};
        projectionLayerViews[i].subImage.imageRect.extent = {isPrimarySwapChain ? primarySwapchainWidth : secondarySwapchainWidth,
                                                             isPrimarySwapChain ? primarySwapchainHeight : secondarySwapchainHeight};
    }

    const auto projectionLayer = reinterpret_cast<XrCompositionLayerProjection*>(xrLayer);
    projectionLayer->space = plugin.inputMgr->GetAppSpace();
    projectionLayer->viewCount = (uint32_t)projectionLayerViews.size();
    projectionLayer->views = projectionLayerViews.data();
}

void YVRProjectionRenderLayer::setAppSWProjectionLayerViewNext()
{
    for (int i = 0; i != 2; ++i)
    {
        if (plugin.renderMgr->configsMgr->isAppSWEnable() && plugin.renderMgr->configsMgr->isAppSWSwitchOn())
        {
            projectionLayerViews[i].next = &spaceWarpInfo[i];
        }
        else
        {
            projectionLayerViews[i].next = NULL;
        }
    }
}

void YVRProjectionRenderLayer::allocSwapChain()
{
    // Enable Foveation
    XrSwapchainCreateInfoFoveationFB swapChainFoveationCreateInfo{XR_TYPE_SWAPCHAIN_CREATE_INFO_FOVEATION_FB};
    swapChainFoveationCreateInfo.flags = plugin.openxrMgr->renderer == kUnityGfxRendererVulkan
                                             ? XR_SWAPCHAIN_CREATE_FOVEATION_FRAGMENT_DENSITY_MAP_BIT_FB
                                             : XR_SWAPCHAIN_CREATE_FOVEATION_SCALED_BIN_BIT_FB;

    xrSwapchainCreateInfo.next = &swapChainFoveationCreateInfo;

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        if (!motionVectorPrimarySwapChain.swapchainAllocated)
        {
            OpenXRAPI(
                xrCreateSwapchain(plugin.openxrMgr->program->session, &motionVectorColorSwapChainCreateInfo, &motionVectorPrimarySwapChain.handle));
            OpenXRAPI(xrEnumerateSwapchainImages(motionVectorPrimarySwapChain.handle, bufferCount, nullptr, motionVectorPrimaryImages));
            motionVectorPrimarySwapChain.swapchainAllocated = true;
        }
        if (!motionVectorPrimaryDepthSwapChain.swapchainAllocated)
        {
            OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &motionVectorDepthSwapChainCreateInfo,
                &motionVectorPrimaryDepthSwapChain.handle));
            OpenXRAPI(xrEnumerateSwapchainImages(motionVectorPrimaryDepthSwapChain.handle, bufferCount, nullptr, motionVectorPrimaryDepthImages));
            motionVectorPrimaryDepthSwapChain.swapchainAllocated = true;
        }

        if (separateSwapChain)
        {
            if (!motionVectorSecondarySwapChain.swapchainAllocated)
            {
                OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &motionVectorColorSwapChainCreateInfo,
                    &motionVectorSecondarySwapChain.handle));
                OpenXRAPI(xrEnumerateSwapchainImages(motionVectorSecondarySwapChain.handle, bufferCount, nullptr, motionVectorSecondaryImages));
                motionVectorSecondarySwapChain.swapchainAllocated = true;
            }

            if (!motionVectorSecondaryDepthSwapChain.swapchainAllocated)
            {
                OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &motionVectorDepthSwapChainCreateInfo,
                    &motionVectorSecondaryDepthSwapChain.handle));
                OpenXRAPI(
                    xrEnumerateSwapchainImages(motionVectorSecondaryDepthSwapChain.handle, bufferCount, nullptr, motionVectorSecondaryDepthImages));
                motionVectorSecondaryDepthSwapChain.swapchainAllocated = true;
            }
        }
    }

    if (!primarySwapchain.swapchainAllocated)
    {
        OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &xrSwapchainCreateInfo, &primarySwapchain.handle));
        OpenXRAPI(xrEnumerateSwapchainImages(primarySwapchain.handle, bufferCount, nullptr, primarySwapchainImages));
        primarySwapchain.swapchainAllocated = true;
    }

    if (separateSwapChain && !secondarySwapchain.swapchainAllocated)
    {
        const float separateRenderScale = plugin.renderMgr->configsMgr->isUsingQuadViews()
                                              ? (depth == 0) ? plugin.renderMgr->configsMgr->getInnerPassResolutionScale()
                                                    : plugin.renderMgr->configsMgr->getInnerExtraPassResolutionScale()
                                              : 1.0f;
        const auto secondRes = plugin.renderMgr->configsMgr->getEyeBufferResolution(1);

        xrSwapchainCreateInfo.width = static_cast<int>(secondRes.x * separateRenderScale);
        xrSwapchainCreateInfo.height = static_cast<int>(secondRes.y * separateRenderScale);
        OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &xrSwapchainCreateInfo, &secondarySwapchain.handle));
        OpenXRAPI(xrEnumerateSwapchainImages(secondarySwapchain.handle, bufferCount, nullptr, secondarySwapchainImages));
        secondarySwapchain.swapchainAllocated = true;
    }

    if (plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
    {
        OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &depthSwapChainCreateInfo, &depthSwapchain.handle))
        OpenXRAPI(xrEnumerateSwapchainImages(depthSwapchain.handle, bufferCount, nullptr, depthSwapchainImages))

        if (separateSwapChain)
        {
            OpenXRAPI(xrCreateSwapchain(plugin.openxrMgr->program->session, &depthSwapChainCreateInfo, &depthSecondarySwapchain.handle))
            OpenXRAPI(xrEnumerateSwapchainImages(depthSecondarySwapchain.handle, bufferCount, nullptr,
                depthSecondarySwapchainImages))
        }
        depthSwapchain.swapchainAllocated = true;
    }

    for (int i = 0; i != plugin.renderMgr->configsMgr->getViewsCount(); ++i)
    {
        projectionLayerViews[i] = {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW};
        projectionLayerViews[i].fov = plugin.hmdMgr->renderViews[i].fov;

        const bool primarySwapChain = plugin.renderMgr->configsMgr->isUsingQuadViews() ? i / 2 == 0 : (separateSwapChain ? i % 2 == 0 : true);

        projectionLayerViews[i].subImage.swapchain = primarySwapChain ? primarySwapchain.handle : secondarySwapchain.handle;
        projectionLayerViews[i].subImage.imageRect.offset = {0, 0};
        projectionLayerViews[i].subImage.imageRect.extent = {i == 0 || !separateSwapChain ? primarySwapchain.width : secondarySwapchain.width,
                                                             i == 0 || !separateSwapChain ? primarySwapchain.height : secondarySwapchain.height};

        if (!separateSwapChain || plugin.renderMgr->configsMgr->isUsingQuadViews())
            projectionLayerViews[i].subImage.imageArrayIndex = i % 2;

        if (plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
        {
            depthInfo[i] = {XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR};
            depthInfo[i].minDepth = 0.0f;
            depthInfo[i].maxDepth = 1.0f;
            depthInfo[i].nearZ = 0.1f;
            depthInfo[i].farZ = 1000.0f;

            depthInfo[i].subImage.swapchain = i == 0 || !separateSwapChain ? depthSwapchain.handle : depthSecondarySwapchain.handle;
            depthInfo[i].subImage.imageRect.offset = {0, 0};
            depthInfo[i].subImage.imageRect.extent = {depthSwapchain.width, depthSwapchain.height};
            if (!separateSwapChain) depthInfo[i].subImage.imageArrayIndex = i;
            projectionLayerViews[i].next = &depthInfo[i];
        }

        if (plugin.renderMgr->configsMgr->isAppSWEnable())
        {
            spaceWarpInfo[i] = {XR_TYPE_COMPOSITION_LAYER_SPACE_WARP_INFO_FB};
            spaceWarpInfo[i].type = XR_TYPE_COMPOSITION_LAYER_SPACE_WARP_INFO_FB;
            spaceWarpInfo[i].next = NULL;
            spaceWarpInfo[i].layerFlags = XR_COMPOSITION_LAYER_SPACE_WARP_INFO_FRAME_SKIP_BIT_FB;

            spaceWarpInfo[i].motionVectorSubImage.swapchain =
                i == 0 || !separateSwapChain ? motionVectorPrimarySwapChain.handle : motionVectorSecondarySwapChain.handle;
            spaceWarpInfo[i].motionVectorSubImage.imageRect.offset.x = 0;
            spaceWarpInfo[i].motionVectorSubImage.imageRect.offset.y = 0;
            spaceWarpInfo[i].motionVectorSubImage.imageRect.extent.width =
                i == 0 || !separateSwapChain ? motionVectorPrimarySwapChain.width : motionVectorSecondarySwapChain.width;
            spaceWarpInfo[i].motionVectorSubImage.imageRect.extent.height =
                i == 0 || !separateSwapChain ? motionVectorPrimarySwapChain.height : motionVectorSecondarySwapChain.height;
            if (!separateSwapChain) spaceWarpInfo[i].motionVectorSubImage.imageArrayIndex = i;

            spaceWarpInfo[i].depthSubImage.swapchain =
                i == 0 || !separateSwapChain ? motionVectorPrimaryDepthSwapChain.handle : motionVectorSecondaryDepthSwapChain.handle;
            spaceWarpInfo[i].depthSubImage.imageRect.offset.x = 0;
            spaceWarpInfo[i].depthSubImage.imageRect.offset.y = 0;
            spaceWarpInfo[i].depthSubImage.imageRect.extent.width =
                i == 0 || !separateSwapChain ? motionVectorPrimaryDepthSwapChain.width : motionVectorSecondaryDepthSwapChain.width;
            spaceWarpInfo[i].depthSubImage.imageRect.extent.height =
                i == 0 || !separateSwapChain ? motionVectorPrimaryDepthSwapChain.height : motionVectorSecondaryDepthSwapChain.height;
            if (!separateSwapChain) spaceWarpInfo[i].depthSubImage.imageArrayIndex = i;

            spaceWarpInfo[i].minDepth = 0.0f;  // 1.0f; //0.0f;
            spaceWarpInfo[i].maxDepth = 1.0f;  // 0.0f; //1.0f
            spaceWarpInfo[i].nearZ = 1000.0f;  // HUGE_VALF;  // 1000.0f; // 0.1f;
            spaceWarpInfo[i].farZ = 0.1f;      // 1000.0f;
        }
    }

    setAppSWProjectionLayerViewNext();

    swapChain();
}

void YVRProjectionRenderLayer::destroySwapChain()
{
    // YVRRenderLayer::destroySwapChain();
    if (primarySwapchain.swapchainAllocated)
    {
        OpenXRAPI(xrDestroySwapchain(primarySwapchain.handle));
        primarySwapchain.swapchainAllocated = false;
        swapchainImageAcquired = false;
    }

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        if (motionVectorPrimarySwapChain.swapchainAllocated)
        {
            OpenXRAPI(xrDestroySwapchain(motionVectorPrimarySwapChain.handle));
            motionVectorPrimarySwapChain.swapchainAllocated = false;
        }

        if (motionVectorPrimaryDepthSwapChain.swapchainAllocated)
        {
            OpenXRAPI(xrDestroySwapchain(motionVectorPrimaryDepthSwapChain.handle));
            motionVectorPrimaryDepthSwapChain.swapchainAllocated = false;
        }

        if (motionVectorSecondarySwapChain.swapchainAllocated)
        {
            OpenXRAPI(xrDestroySwapchain(motionVectorSecondarySwapChain.handle));
            motionVectorSecondarySwapChain.swapchainAllocated = false;
        }

        if (motionVectorSecondaryDepthSwapChain.swapchainAllocated)
        {
            OpenXRAPI(xrDestroySwapchain(motionVectorSecondaryDepthSwapChain.handle));
            motionVectorSecondaryDepthSwapChain.swapchainAllocated = false;
        }
    }

    if (separateSwapChain && secondarySwapchain.swapchainAllocated)
    {
        OpenXRAPI(xrDestroySwapchain(secondarySwapchain.handle));
        secondarySwapchain.swapchainAllocated = false;
        secondarySwapchainImageAcquired = false;
    }

    if (plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
    {
        OpenXRAPI(xrDestroySwapchain(depthSwapchain.handle));
        OpenXRAPI(xrDestroySwapchain(depthSecondarySwapchain.handle));
    }
}

void YVRProjectionRenderLayer::swapChain()
{
    YVRRenderLayer::swapChain();

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        if (motionVectorPrimarySwapChain.swapchainAllocated && !motionVectorPrimaryImageAcquired)
        {
            constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
            OpenXRAPI(xrAcquireSwapchainImage(motionVectorPrimarySwapChain.handle, &acquired, &motionVectorPrimarySwapchainIndex));

            XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
            waitInfo.timeout = XR_INFINITE_DURATION;
            OpenXRAPI(xrWaitSwapchainImage(motionVectorPrimarySwapChain.handle, &waitInfo));

            motionVectorPrimaryImageAcquired = true;
        }

        if (motionVectorPrimaryDepthSwapChain.swapchainAllocated && !motionVectorPrimaryDepthImageAcquired)
        {
            constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
            OpenXRAPI(xrAcquireSwapchainImage(motionVectorPrimaryDepthSwapChain.handle, &acquired, &motionVectorPrimaryDepthSwapchainIndex));

            XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
            waitInfo.timeout = XR_INFINITE_DURATION;
            OpenXRAPI(xrWaitSwapchainImage(motionVectorPrimaryDepthSwapChain.handle, &waitInfo));

            motionVectorPrimaryDepthImageAcquired = true;
        }

        if (separateSwapChain)
        {
            if (motionVectorSecondarySwapChain.swapchainAllocated && !motionVectorSecondaryImageAcquired)
            {
                constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
                OpenXRAPI(xrAcquireSwapchainImage(motionVectorSecondarySwapChain.handle, &acquired, &motionVectorSecondarySwapchainIndex));

                XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
                waitInfo.timeout = XR_INFINITE_DURATION;
                OpenXRAPI(xrWaitSwapchainImage(motionVectorSecondarySwapChain.handle, &waitInfo));

                motionVectorSecondaryImageAcquired = true;
            }

            if (motionVectorSecondaryDepthSwapChain.swapchainAllocated && !motionVectorSecondaryDepthImageAcquired)
            {
                constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
                OpenXRAPI(xrAcquireSwapchainImage(motionVectorSecondaryDepthSwapChain.handle, &acquired, &motionVectorSecondaryDepthSwapchainIndex));

                XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
                waitInfo.timeout = XR_INFINITE_DURATION;
                OpenXRAPI(xrWaitSwapchainImage(motionVectorSecondaryDepthSwapChain.handle, &waitInfo));

                motionVectorSecondaryDepthImageAcquired = true;
            }
        }
    }

    if (separateSwapChain && secondarySwapchain.swapchainAllocated && !secondarySwapchainImageAcquired)
    {
        constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
        OpenXRAPI(xrAcquireSwapchainImage(secondarySwapchain.handle, &acquired, &secondarySwapchainIndex));

        XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
        waitInfo.timeout = XR_INFINITE_DURATION;
        OpenXRAPI(xrWaitSwapchainImage(secondarySwapchain.handle, &waitInfo));

        secondarySwapchainImageAcquired = true;
    }

    if (plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer())
    {
        constexpr XrSwapchainImageAcquireInfo acquired{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
        OpenXRAPI(xrAcquireSwapchainImage(depthSwapchain.handle, &acquired, &depthSwapchainIndex));

        XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
        waitInfo.timeout = XR_INFINITE_DURATION;
        OpenXRAPI(xrWaitSwapchainImage(depthSwapchain.handle, &waitInfo));

        if (separateSwapChain)
        {
            OpenXRAPI(xrAcquireSwapchainImage(depthSecondarySwapchain.handle, &acquired, &depthSecondarySwapchainIndex));
            OpenXRAPI(xrWaitSwapchainImage(depthSecondarySwapchain.handle, &waitInfo));
        }

        depthImageAcquired = true;
    }
}

std::uintptr_t YVRProjectionRenderLayer::getColorHandle(xrEyeSide eyeSide, int bufferIndex)
{
    std::uintptr_t colorHandle;

    if (eyeSide == xrEyeSide::kEyeMaskBoth || eyeSide == xrEyeSide::kEyeMaskLeft)
        colorHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(primarySwapchainImages, bufferIndex);
    else
        colorHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(secondarySwapchainImages, bufferIndex);

        return colorHandle;
}

std::uintptr_t YVRProjectionRenderLayer::getDepthHandle(xrEyeSide eyeSide, int bufferIndex)
{
    std::uintptr_t depthHandle = -1;
    if (!plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer()) return depthHandle;

    if (eyeSide == xrEyeSide::kEyeMaskBoth || eyeSide == xrEyeSide::kEyeMaskLeft)
        depthHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(depthSwapchainImages, bufferIndex);
    else
        depthHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(depthSecondarySwapchainImages, bufferIndex);

    return depthHandle;
}

std::uintptr_t YVRProjectionRenderLayer::getMotionVectorColorHandle(xrEyeSide eyeSide, int bufferIndex)
{
    std::uintptr_t colorHandle = -1;
    if (!plugin.renderMgr->configsMgr->isAppSWEnable()) return colorHandle;

    if (eyeSide == xrEyeSide::kEyeMaskBoth || eyeSide == xrEyeSide::kEyeMaskLeft)
        colorHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(motionVectorPrimaryImages, bufferIndex);
    else
        colorHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(motionVectorSecondaryImages, bufferIndex);

    return colorHandle;
}

std::uintptr_t YVRProjectionRenderLayer::getMotionVectorDepthHandle(xrEyeSide eyeSide, int bufferIndex)
{
    std::uintptr_t depthHandle = -1;
    if (!plugin.renderMgr->configsMgr->isAppSWEnable()) return depthHandle;

    if (eyeSide == xrEyeSide::kEyeMaskBoth || eyeSide == xrEyeSide::kEyeMaskLeft)
        depthHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(motionVectorPrimaryDepthImages, bufferIndex);
    else
        depthHandle = plugin.openxrMgr->graphicsPlugin->getImageHandle(motionVectorSecondaryDepthImages, bufferIndex);

    return depthHandle;
}

uint32_t YVRProjectionRenderLayer::getMotionVectorImageRectWidth() { return recommendedMotionVectorImageRectWidth; }

uint32_t YVRProjectionRenderLayer::getMotionVectorImageRectHeight() { return recommendedMotionVectorImageRectHeight; }

void YVRProjectionRenderLayer::setFoveationLevel(XrFoveationLevelFB level, XrFoveationDynamicFB dynamic)
{
    AnnounceCalledFunc();
    YDebug("Set FFR Level is %d, %d", level, dynamic);
    XrFoveationLevelProfileCreateInfoFB levelProfileCreateInfo{XR_TYPE_FOVEATION_LEVEL_PROFILE_CREATE_INFO_FB};
    levelProfileCreateInfo.level = level;
    levelProfileCreateInfo.verticalOffset = 0;
    levelProfileCreateInfo.dynamic = dynamic;

    XrFoveationProfileCreateInfoFB profileCreateInfo{XR_TYPE_FOVEATION_PROFILE_CREATE_INFO_FB};
    profileCreateInfo.next = &levelProfileCreateInfo;

    XrFoveationProfileFB foveationProfile;
    xrCreateFoveationProfileFB(plugin.openxrMgr->program->session, &profileCreateInfo, &foveationProfile);

    XrSwapchainStateFoveationFB foveationUpdateState{XR_TYPE_SWAPCHAIN_STATE_FOVEATION_FB};
    foveationUpdateState.profile = foveationProfile;

    xrUpdateSwapchainFB(primarySwapchain.handle, reinterpret_cast<XrSwapchainStateBaseHeaderFB*>(&foveationUpdateState));
    if (separateSwapChain) xrUpdateSwapchainFB(secondarySwapchain.handle, reinterpret_cast<XrSwapchainStateBaseHeaderFB*>(&foveationUpdateState));
}

void YVRProjectionRenderLayer::setCurrentPosePosition(float x, float y, float z)
{
    currentPose.position.x = x;
    currentPose.position.y = y;
    currentPose.position.z = -z;
}

void YVRProjectionRenderLayer::setCurrentPoseRotation(float x, float y, float z, float w)
{
    currentPose.orientation.x = -x;
    currentPose.orientation.y = -y;
    currentPose.orientation.z = z;
    currentPose.orientation.w = w;
}