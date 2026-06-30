#include "YVRDisplayProvider.h"

#include "../Utilities/YVRCoordinateConverter.h"
#include "../YVRModuleMgrs/Render/RenderTasks/SetFoveationDataEventTask.h"
#include "../YVRModuleMgrs/Render/RenderTasks/SetEyeBufferSettingsTask.h"
#include "../Utilities/YVRProfiler.h"

YVRDisplayProvider::YVRDisplayProvider(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle)
    : YVRProviderBase(context, handle),
      unityTextureDescArray{},
      unityTextureIDArray{}, unityEnvironmentTextureDescArray(), unityEnvironmentTextureIDArray(),
      unityMotionVectorTextureDescArray{},
      unityMotionVectorTextureIDArray{}
{}

void YVRDisplayProvider::initializeUnityGfxThreadLifecycle(UnityXRDisplayGraphicsThreadProvider& unityGfxThreadProvider)
{
    unityGfxThreadProvider.Start = [](UnitySubsystemHandle handle, void* userData,
                                      UnityXRRenderingCapabilities* renderingCaps) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.displayProvider->onGfxThreadStart(renderingCaps);
    };

    unityGfxThreadProvider.SubmitCurrentFrame = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        BeginSample(yvrProviderContext.displayProvider->submitCurrentFrameMarker);
        const auto ret = yvrProviderContext.displayProvider->onGfxThreadSubmitCurrentFrame();
        EndSample(yvrProviderContext.displayProvider->submitCurrentFrameMarker);
        return ret;
    };

    unityGfxThreadProvider.PopulateNextFrameDesc = [](UnitySubsystemHandle handle, void* userData, const UnityXRFrameSetupHints* frameHints,
                                                      UnityXRNextFrameDesc* nextFrame) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        BeginSample(yvrProviderContext.displayProvider->populateNextFrameDescMarker);
        const auto ret = yvrProviderContext.displayProvider->onGfxThreadPopulateNextFrameDesc(frameHints, nextFrame);
        EndSample(yvrProviderContext.displayProvider->populateNextFrameDescMarker);
        return ret;
    };

    unityGfxThreadProvider.Stop = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.displayProvider->onGfxThreadStop();
    };
}

UnitySubsystemErrorCode YVRDisplayProvider::initialize()
{
    AnnounceCallingFunc();

    // Register Unity Graphic related called
    UnityXRDisplayGraphicsThreadProvider unityGfxThreadProvider{};
    unityGfxThreadProvider.userData = &m_Context;
    initializeUnityGfxThreadLifecycle(unityGfxThreadProvider);
    m_Context.displayInterface->RegisterProviderForGraphicsThread(m_SystemHandle, &unityGfxThreadProvider);

    CreateMarker(&populateNextFrameDescMarker, "NativeCore-PopulateNextFrameDescMarker");
    CreateMarker(&submitCurrentFrameMarker, "NativeCore-SubmitCurrentFrameMarker");

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::start()
{
    AnnounceCallingFunc();
    return kUnitySubsystemErrorCodeSuccess;
}

void YVRDisplayProvider::stop()
{
    AnnounceCallingFunc();
}

void YVRDisplayProvider::shutDown()
{
    AnnounceCallingFunc();

    plugin.lifecycleMgr->onUnityXRGfxStop();
}

bool textureInitialize = false;

UnitySubsystemErrorCode YVRDisplayProvider::onGfxThreadStart(UnityXRRenderingCapabilities* renderingCaps)
{
    AnnounceCallingFunc();

    if (startNeverCalled)
    {
        plugin.lifecycleMgr->onUnityXRGfxStart();
        startNeverCalled = false;
    }

    plugin.lifecycleMgr->onUnityGfxThreadResume();

    renderingCaps->supportedTextureLayoutFlags =
        UnityXRTextureLayoutFlags::kUnityXRTextureLayoutFlagsTexture2DArray | UnityXRTextureLayoutFlags::kUnityXRTextureLayoutFlagsSeparateTexture2Ds;
    //renderingCaps->noSinglePassRenderingSupport = false;
    renderingCaps->invalidateRenderStateAfterEachCallback = true; // Invalid Unity set render state, as the native plugin will modify some state
    // Let Unity not call swap chain while not in development build mode to save performance
    renderingCaps->skipPresentToMainScreen = !plugin.developmentBuildMode;

    if (plugin.renderMgr->configsMgr->passthroughProviderEnable())
    {
        plugin.passThroughProviderMgr->createPassthroughProvider();
        plugin.passThroughProviderMgr->allocSwapChain();
        plugin.passThroughProviderMgr->startPassthroughProvider();
    }

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::onGfxThreadSubmitCurrentFrame()
{
    // Skip first
    plugin.lifecycleMgr->onPreUnityGfxSubmitCurrentFrame();

    if (plugin.renderMgr->shouldSkipFrame())
        plugin.renderMgr->skipFrame();
    else
        plugin.renderMgr->submitFrame();

    plugin.lifecycleMgr->onPostUnityGfxSubmitCurrentFrame();

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::onGfxThreadPopulateNextFrameDesc(const UnityXRFrameSetupHints* frameHints,
                                                                             UnityXRNextFrameDesc* nextFrame)
{
    plugin.lifecycleMgr->onPreUnityGfxPrepareNextFrame();

    // As the render scale may set by Unity URP in the first frame, the render scale data got in the first frame may be wrong.
    // Thus, set the swap chain data in the second frame.
    if (plugin.lifecycleMgr->getSubmittedFrameCount() == 0) { return kUnitySubsystemErrorCodeSuccess; }

    plugin.renderMgr->WaitFrame();
    plugin.renderMgr->BeginFrame();

    plugin.hmdMgr->refreshViews(true);

    if (!plugin.lifecycleMgr->isVisible())
    {
        nextFrame->renderPassesCount = 0;
        return kUnitySubsystemErrorCodeSuccess;
    }

    const bool multiPasses = !plugin.renderMgr->configsMgr->isUsingQuadViews() && !plugin.renderMgr->configsMgr->isUsingSinglePass();
    const int renderParamsCount = multiPasses ? 1 : 2;
    int renderPassCount = plugin.renderMgr->configsMgr->isUsingSinglePass() ? 1 : 2;
    renderPassCount = plugin.renderMgr->configsMgr->hasExtraRenderPass() ? renderPassCount * 2 : renderPassCount;

    nextFrame->renderPassesCount = renderPassCount;
    for (int i = 0; i != renderPassCount; ++i)
    {
        nextFrame->renderPasses[i].renderParamsCount = renderParamsCount;
    }

    if (plugin.renderMgr->configsMgr->getResolutionScale() != frameHints->appSetup.textureResolutionScale)
    {
        YInfo("setResolutionScale %f", frameHints->appSetup.textureResolutionScale);
        plugin.renderMgr->configsMgr->setResolutionScale(frameHints->appSetup.textureResolutionScale);
        DestroyUnityTexture();
    }

    plugin.renderMgr->configsMgr->setRenderViewport({frameHints->appSetup.renderViewport.width, frameHints->appSetup.renderViewport.height});
    if (!textureInitialize)
    {
        const XrColorSpaceYVR colorspace = plugin.renderMgr->configsMgr->getColorSpace();
        YInfo("Use color space is %d", colorspace);
        OpenXRAPI(xrSetColorSpaceYVR(plugin.openxrMgr->program->session,colorspace));
        if (plugin.renderMgr->configsMgr->isP3())
        {
            YInfo("Set P3 Color Space");
            OpenXRAPI(xrSetColorSpaceFB(plugin.openxrMgr->program->session,XrColorSpaceFB::XR_COLOR_SPACE_P3_FB));
        }

        initializeUnityTexture();
        textureInitialize = true;
        plugin.gfxTasksMgr->PushOnPreSubmitTask(
            new SetFoveationDataEventTask(plugin.renderMgr->configsMgr->getFFRLevel(), plugin.renderMgr->configsMgr->getFFRDynamic()));
        YVRLayerSettings yvrLayerSettings = plugin.renderMgr->configsMgr->getEyeBufferLayerSettings();
        plugin.gfxTasksMgr->PushOnPreSubmitTask(new SetEyeBufferSettingsTask(yvrLayerSettings.enableSuperSample,
                                                                             yvrLayerSettings.expensiveSuperSample,
                                                                             yvrLayerSettings.enableSharpen,
                                                                             yvrLayerSettings.expensiveSharpen));
    }

    updateRenderPasses(nextFrame);
    updateCullPasses(nextFrame);
    swapUnityRenderBuffer(nextFrame);
    if (plugin.renderMgr->configsMgr->passthroughProviderEnable())
    {
        plugin.passThroughProviderMgr->swapChain();
    }
    if (plugin.passThroughProviderMgr->largeXRPassthroughProvider != XR_NULL_HANDLE)
    {
        plugin.passThroughProviderMgr->largePassthrouthSteamSwapChain();
    }
    

    plugin.lifecycleMgr->onPostUnityGfxPrepareNextFrame();

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::onGfxThreadStop()
{
    AnnounceCallingFunc();
    plugin.lifecycleMgr->onUnityGfxThreadPause();

    DestroyUnityTexture();

    if (plugin.renderMgr->configsMgr->passthroughProviderEnable())
    {
        plugin.passThroughProviderMgr->stopPassthroughProvider();
        plugin.passThroughProviderMgr->destroySwapChain();
        plugin.passThroughProviderMgr->destroyPassthroughProvider();
    }

    return kUnitySubsystemErrorCodeSuccess;
}

void YVRDisplayProvider::DestroyUnityTexture()
{
    if (textureInitialize)
    {
        plugin.renderMgr->renderLayersMgr->deleteEyeBufferLayer(false);
        if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
        {
            plugin.renderMgr->renderLayersMgr->deleteEnvironmentBufferLayer(false);
        }
        destroyTextures();
    }
}


void YVRDisplayProvider::initializeRenderLayer()
{
    AnnounceCallingFunc();
    const xrTextureType textureType = plugin.renderMgr->configsMgr->isUsingSinglePass() || plugin.renderMgr->configsMgr->isUsingQuadViews()
                                          ? kTypeTextureArray : kTypeTexture;

    const XrVector2f resolution = plugin.renderMgr->configsMgr->getEyeBufferResolution();
    const LayerCreateInfo layerCreateInfo{
        0, static_cast<int>(resolution.x), static_cast<int>(resolution.y), SWAP_BUFFER_NUM, textureType, kLayerFlagOpaque, -1,
        !plugin.renderMgr->configsMgr->isUsingSinglePass(), true,
        false, Projection, kEyeMaskBoth};

    const int layerId = plugin.renderMgr->renderLayersMgr->createLayer(layerCreateInfo);
    plugin.renderMgr->renderLayersMgr->addActiveLayer(layerId);

    if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
    {
        const LayerCreateInfo environmentLayerCreateInfo{plugin.renderMgr->configsMgr->getExtraRenderPassDepth(),
                                                         static_cast<int>(resolution.x),
                                                         static_cast<int>(resolution.y),
                                                         SWAP_BUFFER_NUM, textureType, kLayerFlagOpaque,
                                                         -1, !plugin.renderMgr->configsMgr->isUsingSinglePass(),
                                                         true, false, Projection, kEyeMaskBoth};
        int environmentLayerId = plugin.renderMgr->renderLayersMgr->createLayer(environmentLayerCreateInfo);
        plugin.renderMgr->renderLayersMgr->addActiveLayer(environmentLayerId);
    }
}

void YVRDisplayProvider::initializeUnityTexture()
{
    AnnounceCallingFunc();

    initializeRenderLayer();

    // If use single pass, only unityTextureDescArray[0] will be used
    for (int i = 0; i != EYE_NUM; ++i)
    {
        const XrVector2f resolution = plugin.renderMgr->configsMgr->getEyeBufferResolution(i);

        unityTextureDescArray[i] = {};
        const float extraScale = plugin.renderMgr->configsMgr->isUsingQuadViews()
                                     ? (i == 0 ? plugin.renderMgr->configsMgr->getOuterPassResolutionScale()
                                            : plugin.renderMgr->configsMgr->getInnerPassResolutionScale())
                                     : 1.0f;
        unityTextureDescArray[i].width = resolution.x * extraScale;
        unityTextureDescArray[i].height = resolution.y * extraScale;
        unityTextureDescArray[i].flags = 0;

        // Do not set auto resolve will leads to msaa not work, here we always set auto resolve for inner fov pass of quad view rendering
        if (plugin.renderMgr->configsMgr->isAutoResolve() || (plugin.renderMgr->configsMgr->isUsingQuadViews() && i != 0))
            unityTextureDescArray[i].flags |= UnityXRRenderTextureFlags::kUnityXRRenderTextureFlagsMultisampleAutoResolve;

        if (plugin.renderMgr->configsMgr->optimizeBufferDiscardsEnable())
        {
            unityTextureDescArray[i].flags |= UnityXRRenderTextureFlags::kUnityXRRenderTextureFlagsOptimizeBufferDiscards;
        }

        if (plugin.renderMgr->configsMgr->getColorSpace() == XR_COLOR_SPACE_SRGB_YVR)
        {
            unityTextureDescArray[i].flags |= kUnityXRRenderTextureFlagsSRGB;
        }

        unityTextureDescArray[i].depthFormat = UnityXRDepthTextureFormat::kUnityXRDepthTextureFormatNone;

        if (plugin.renderMgr->configsMgr->isUsingSinglePass() || plugin.renderMgr->configsMgr->isUsingQuadViews())
            unityTextureDescArray[i].textureArrayLength = 2;

        if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
        {
            const float extraRenderPassScale = plugin.renderMgr->configsMgr->isUsingQuadViews() ?
                                                   (i == 0 ? plugin.renderMgr->configsMgr->getOuterExtraPassResolutionScale() :
                                                        plugin.renderMgr->configsMgr->getInnerExtraPassResolutionScale())
                                                   : 1.0;

            unityEnvironmentTextureDescArray[i] = {};
            unityEnvironmentTextureDescArray[i].width = resolution.x * extraRenderPassScale;
            unityEnvironmentTextureDescArray[i].height = resolution.y * extraRenderPassScale;
            unityEnvironmentTextureDescArray[i].flags = unityTextureDescArray[i].flags;
            unityEnvironmentTextureDescArray[i].depthFormat = unityTextureDescArray[i].depthFormat;
            unityEnvironmentTextureDescArray[i].textureArrayLength = unityTextureDescArray[i].textureArrayLength;
        }

        if (plugin.renderMgr->configsMgr->isAppSWEnable())
        {
            unityMotionVectorTextureDescArray[i] = {};
            unityMotionVectorTextureDescArray[i].width = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getMotionVectorImageRectWidth();
            unityMotionVectorTextureDescArray[i].height = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getMotionVectorImageRectHeight();
            unityMotionVectorTextureDescArray[i].flags = UnityXRRenderTextureFlags::kUnityXRRenderTextureFlagsMotionVectorTexture;
            unityMotionVectorTextureDescArray[i].colorFormat = UnityXRRenderTextureFormat::kUnityXRRenderTextureFormatR16G16B16A16_SFloat;
            unityMotionVectorTextureDescArray[i].depthFormat = UnityXRDepthTextureFormat::kUnityXRDepthTextureFormat24bitOrGreater;

            if (plugin.renderMgr->configsMgr->isUsingSinglePass()) unityMotionVectorTextureDescArray[i].textureArrayLength = 2;
        }
    }

    const int requiredUnityTexture = plugin.renderMgr->configsMgr->isUsingSinglePass() ? SWAP_BUFFER_NUM : (EYE_NUM * SWAP_BUFFER_NUM);
    for (int i = 0; i != requiredUnityTexture; ++i)
    {
        UnityXRRenderTextureDesc unityTextureDesc = unityTextureDescArray[i / SWAP_BUFFER_NUM];
        const xrEyeSide eyeMask = i < SWAP_BUFFER_NUM ? xrEyeSide::kEyeMaskLeft : xrEyeSide::kEyeMaskRight;
        const uintptr_t nativeColorID = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getColorHandle(eyeMask, i % SWAP_BUFFER_NUM);
        const uintptr_t nativeDepthID = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getDepthHandle(eyeMask, i % SWAP_BUFFER_NUM);
        unityTextureDesc.color.nativePtr = (void*)(nativeColorID);
        unityTextureDesc.depth.nativePtr = plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer() ? (void*)(nativeDepthID)
                                               : (void*)kUnityXRRenderTextureIdDontCare;
        UnityXRAPI(m_Context.displayInterface->CreateTexture(m_SystemHandle, &unityTextureDesc, &unityTextureIDArray[i]));
    }

    if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
    {
        for (int i = 0; i != requiredUnityTexture; ++i)
        {
            UnityXRRenderTextureDesc unityTextureDesc = unityEnvironmentTextureDescArray[i / SWAP_BUFFER_NUM];
            const xrEyeSide eyeMask = i < SWAP_BUFFER_NUM ? xrEyeSide::kEyeMaskLeft : xrEyeSide::kEyeMaskRight;
            const uintptr_t nativeColorID = plugin.renderMgr->renderLayersMgr->getEnvironmentLayer()->getColorHandle(eyeMask, i % SWAP_BUFFER_NUM);
            const uintptr_t nativeDepthID = plugin.renderMgr->renderLayersMgr->getEnvironmentLayer()->getDepthHandle(eyeMask, i % SWAP_BUFFER_NUM);
            unityTextureDesc.color.nativePtr = (void*)(nativeColorID);
            unityTextureDesc.depth.nativePtr = plugin.renderMgr->configsMgr->isUsingNativeDepthBuffer() ? (void*)(nativeDepthID)
                                                   : (void*)kUnityXRRenderTextureIdDontCare;
            UnityXRAPI(m_Context.displayInterface->CreateTexture(m_SystemHandle, &unityTextureDesc, &unityEnvironmentTextureIDArray[i]));
        }
    }

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        for (int i = 0; i != requiredUnityTexture; ++i)
        {
            UnityXRRenderTextureDesc unityMotionVectorTextureDesc = unityMotionVectorTextureDescArray[i / SWAP_BUFFER_NUM];
            const xrEyeSide eyeMask = i < SWAP_BUFFER_NUM ? xrEyeSide::kEyeMaskLeft : xrEyeSide::kEyeMaskRight;
            const uintptr_t nativeColorID = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getMotionVectorColorHandle(
                eyeMask, i % SWAP_BUFFER_NUM);
            const uintptr_t nativeDepthID = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getMotionVectorDepthHandle(
                eyeMask, i % SWAP_BUFFER_NUM);
            unityMotionVectorTextureDesc.color.nativePtr = (void*)(nativeColorID);
            unityMotionVectorTextureDesc.depth.nativePtr = (void*)(nativeDepthID);
            UnityXRAPI(m_Context.displayInterface->CreateTexture(m_SystemHandle, &unityMotionVectorTextureDesc, &unityMotionVectorTextureIDArray[i]));
        }
    }
}

void YVRDisplayProvider::destroyTextures()
{
    const int requiredUnityTexture = plugin.renderMgr->configsMgr->isUsingSinglePass() ? SWAP_BUFFER_NUM : (EYE_NUM * SWAP_BUFFER_NUM);

    for (int i = 0; i != requiredUnityTexture; ++i)
    {
        UnityXRAPI(m_Context.displayInterface->DestroyTexture(m_SystemHandle, unityTextureIDArray[i]));
    }

    if (plugin.renderMgr->configsMgr->isAppSWEnable())
    {
        for (int i = 0; i != requiredUnityTexture; ++i)
        {
            UnityXRAPI(m_Context.displayInterface->DestroyTexture(m_SystemHandle, unityMotionVectorTextureIDArray[i]));
        }
    }

    if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
    {
        for (int i = 0; i != requiredUnityTexture; ++i)
        {
            UnityXRAPI(m_Context.displayInterface->DestroyTexture(m_SystemHandle, unityEnvironmentTextureIDArray[i]));
        }
    }

    textureInitialize = false;
}


void YVRDisplayProvider::swapUnityRenderBuffer(UnityXRNextFrameDesc* nextFrameDesc)
{
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->swapChain();
    const int bufferIndex = plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->getBufferIndex();
    const int renderPassCount = plugin.renderMgr->configsMgr->isUsingSinglePass() ? 1 : EYE_NUM;

    int mainPassOffset = 0;
    if (plugin.renderMgr->configsMgr->hasExtraRenderPass())
    {
        plugin.renderMgr->renderLayersMgr->getEnvironmentLayer()->swapChain();
        const int environmentBufferIndex = plugin.renderMgr->renderLayersMgr->getEnvironmentLayer()->getBufferIndex();
        for (int i = 0; i != renderPassCount; ++i)
        {
            nextFrameDesc->renderPasses[i].textureId = unityEnvironmentTextureIDArray[i * SWAP_BUFFER_NUM + environmentBufferIndex];
        }
        mainPassOffset = renderPassCount;
    }

    for (int i = 0; i != renderPassCount; ++i)
    {
        nextFrameDesc->renderPasses[mainPassOffset + i].textureId = unityTextureIDArray[i * SWAP_BUFFER_NUM + bufferIndex];
        if (plugin.renderMgr->configsMgr->isAppSWEnable() && plugin.renderMgr->configsMgr->isAppSWSwitchOn())
        {
            nextFrameDesc->renderPasses[mainPassOffset + i].motionVectorTextureId = unityMotionVectorTextureIDArray[
                i * SWAP_BUFFER_NUM + bufferIndex];
        }
        else
        {
            nextFrameDesc->renderPasses[mainPassOffset + i].motionVectorTextureId = 0;
        }
    }
}

void YVRDisplayProvider::updateRenderPasses(UnityXRNextFrameDesc* nextFrameDesc)
{
    const auto configsMgr = plugin.renderMgr->configsMgr;

    const bool isUsingQuadViews = configsMgr->isUsingQuadViews();
    const bool hasExtraRenderPass = configsMgr->hasExtraRenderPass();
    const bool isUsingSinglePass = configsMgr->isUsingSinglePass();
    const bool isMonoscopic = configsMgr->isMonoscopic();
    const int viewsCount = configsMgr->getViewsCount();
    const auto renderViewport = configsMgr->getRenderViewport();

    int totalRenderViews = isUsingQuadViews ? 2 * EYE_NUM : EYE_NUM;
    totalRenderViews = hasExtraRenderPass ? 2 * totalRenderViews : totalRenderViews;
    const bool multiPasses = !isUsingQuadViews && !isUsingSinglePass;

    const std::vector<XrView> views = plugin.hmdMgr->renderViews;
    for (int i = 0; i != totalRenderViews; ++i)
    {
        int renderPassIndex = multiPasses ? i : i / 2;
        int renderParamsIndex = multiPasses ? 0 : i % 2;

        auto& renderParams = nextFrameDesc->renderPasses[renderPassIndex].renderParams[renderParamsIndex];

        UnityXRPose eyeRelatedPose;

        if (!isMonoscopic)
        {
            const int biasIndex = multiPasses ? renderPassIndex : renderParamsIndex;
            eyeRelatedPose = OpenXRPose2UnityPose(views[biasIndex].pose);
        }
        else
        {
            eyeRelatedPose = {UnityXRVector3{}, UnityXRVector4{0, 0, 0, 1}};
        }

        renderParams.deviceAnchorToEyePose = eyeRelatedPose;
        renderParams.viewportRect = {0, 0, renderViewport.x, renderViewport.y};

        if (isUsingSinglePass || isUsingQuadViews)
            renderParams.textureArraySlice = renderParamsIndex;

        const int fovIndex = i % viewsCount;
        const auto& eyeFov = views[fovIndex].fov;

        renderParams.projection.type = UnityXRProjectionType::kUnityXRProjectionTypeHalfAngles;
        renderParams.projection.data.halfAngles.left = tan(eyeFov.angleLeft);
        renderParams.projection.data.halfAngles.right = tan(eyeFov.angleRight);
        renderParams.projection.data.halfAngles.top = tan(eyeFov.angleUp);
        renderParams.projection.data.halfAngles.bottom = tan(eyeFov.angleDown);
    }
}

void YVRDisplayProvider::updateCullPasses(UnityXRNextFrameDesc* nextFrameDesc)
{
    // Configure the CullingPass at index 0

    nextFrameDesc->cullingPasses[0].separation = plugin.renderMgr->configsMgr->ipd / 2.0f;
    nextFrameDesc->cullingPasses[0].deviceAnchorToCullingPose = nextFrameDesc->renderPasses[0].renderParams[0].deviceAnchorToEyePose;
    auto projection = nextFrameDesc->renderPasses[0].renderParams[0].projection;
    const XrFovf& eyeFov = plugin.hmdMgr->renderViews[0].fov;
    projection.data.halfAngles.right = tan(eyeFov.angleRight * 1.2f);
    nextFrameDesc->cullingPasses[0].projection = projection;

    // Both left and right eye use the CullingPass at index 0
    int renderPassCount = plugin.renderMgr->configsMgr->isUsingSinglePass() ? 1 : 2;
    renderPassCount = plugin.renderMgr->configsMgr->hasExtraRenderPass() ? renderPassCount * 2 : renderPassCount;
    for (int i = 0; i != renderPassCount; ++i)
    {
        nextFrameDesc->renderPasses[i].cullingPassIndex = 0;
    }
}

YVRDisplayProvider::~YVRDisplayProvider() = default;
