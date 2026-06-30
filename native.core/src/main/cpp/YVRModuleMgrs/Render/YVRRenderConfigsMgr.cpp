#include "YVRRenderConfigsMgr.h"
#include "../../UnityHeader/XR/IUnityXRInput.h"
#include "../../YVRUnityPlugin.h"

YVRRenderConfigsMgr::YVRRenderConfigsMgr() = default;

YVRRenderConfigsMgr::~YVRRenderConfigsMgr() = default;


void YVRRenderConfigsMgr::refreshSystemProperty(bool force)
{
    if (!force && !plugin.lifecycleMgr->isInVrMode()) return;

    ipd = plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_IPD);
}

bool YVRRenderConfigsMgr::isUsingQuadViews() const
{
    return this->renderMode == QuadViews && plugin.openxrMgr->program->supportQuadViews;
}

bool YVRRenderConfigsMgr::isUsingSinglePass() const
{
    return this->renderMode == SinglePassMultiView ||
           (this->renderMode == QuadViews && !plugin.openxrMgr->program->supportQuadViews);
}

YVRStereoRenderMode YVRRenderConfigsMgr::getActualRenderingMode()
{
    if (isUsingQuadViews()) return QuadViews;
    if (isUsingSinglePass()) return SinglePassMultiView;
    return MultiPasses;
}

int YVRRenderConfigsMgr::getViewsCount() const { return isUsingQuadViews() ? 4 : 2; }

const int& YVRRenderConfigsMgr::getDisplayRate()
{
    if (displayRate == -1 && plugin.lifecycleMgr->isInVrMode())
    {
        setDisplayRate(defaultDisplayRate);
    }
    float ret{};
    OpenXRAPI(xrGetDisplayRefreshRateFB(plugin.openxrMgr->program->session, &ret));

    displayRate = static_cast<int>(ret);
    return displayRate;
}

void YVRRenderConfigsMgr::setDisplayRate(const int& displayRate)
{
    if (displayRate == -1) return;
    this->displayRate = displayRate;
    if (plugin.lifecycleMgr->isInVrMode())
    {
        OpenXRAPI(xrRequestDisplayRefreshRateFB(plugin.openxrMgr->program->session, displayRate));
    }
}

const bool& YVRRenderConfigsMgr::isMonoscopic() { return monoscopic; }
void YVRRenderConfigsMgr::setMonoscopic(const bool& isMonoscopic) { monoscopic = isMonoscopic; }

const bool& YVRRenderConfigsMgr::isAutoResolve() { return autoResolve; }
void YVRRenderConfigsMgr::setAutoResolve(const bool& autoResolve) { this->autoResolve = autoResolve; }

const bool& YVRRenderConfigsMgr::hasExtraRenderPass() { return extraRenderPass; }
void YVRRenderConfigsMgr::setExtraRenderPass(const bool& extraRenderPass) { this->extraRenderPass = extraRenderPass; }

const bool& YVRRenderConfigsMgr::hasExtraLatency() { return extraLatency; }

void YVRRenderConfigsMgr::setExtraLatency(const bool& extraLatency)
{
    const XrResult result =
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_EXTRA_LATENCY_MODE, extraLatency ? 1 : 0));

    this->extraLatency = extraLatency && result == XR_SUCCESS;
}

const bool& YVRRenderConfigsMgr::isAppSWEnable() { return appSWEnabled; }
void YVRRenderConfigsMgr::setAppSWEnable(const bool& enable) { this->appSWEnabled = enable; }

const bool& YVRRenderConfigsMgr::isAppSWSwitchOn() { return appSWSwitch; }

void YVRRenderConfigsMgr::setAppSWSwitch(const bool& isOn)
{
    this->appSWSwitch = isOn;
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->setAppSWProjectionLayerViewNext();
}

const bool& YVRRenderConfigsMgr::optimizeBufferDiscardsEnable() { return optimizeBufferDiscards; }

void YVRRenderConfigsMgr::setOptimizeBufferDiscards(const bool& isEnable) { this->optimizeBufferDiscards = isEnable; }

const bool& YVRRenderConfigsMgr::isUsingNativeDepthBuffer() { return useNativeDepthBuffer; }
void YVRRenderConfigsMgr::setUseNativeDepthBuffer(const bool& useNativeDepthBuffer) { this->useNativeDepthBuffer = useNativeDepthBuffer; }

const bool& YVRRenderConfigsMgr::passthroughProviderEnable() { return enablePassthroughProvider; }

void YVRRenderConfigsMgr::setPassthroughProviderEnable(const bool& isEnable) { this->enablePassthroughProvider = isEnable; }

bool YVRRenderConfigsMgr::requireAllTransLayer() { return systemMenuMode | passThroughMode; }

void YVRRenderConfigsMgr::setSystemMenuMode(const bool& enable)
{
    systemMenuMode = enable;
    if (systemMenuMode)
    {
        frameEndInfoLayerTypeExt.layerType = YVR_LAYER_TYPE_GLOBAL_SYSTEM_MENU;
    }
    frameEndInfoExt.next = enable ? &frameEndInfoLayerTypeExt : nullptr;
    plugin.renderMgr->renderLayersMgr->SetLayersVecRequiredReSort();
}

void YVRRenderConfigsMgr::setPassThroughMode(const bool& enable)
{
    passThroughMode = true;
    plugin.renderMgr->renderLayersMgr->SetLayersVecRequiredReSort();
}

const YVRLayerSettings& YVRRenderConfigsMgr::getEyeBufferLayerSettings() { return eyeBufferLayerSettings; }

void YVRRenderConfigsMgr::setEyeBufferLayerSettings(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen)
{
    eyeBufferLayerSettings.enableSuperSample = enableSuperSample;
    eyeBufferLayerSettings.expensiveSuperSample = expensiveSuperSample;
    eyeBufferLayerSettings.enableSharpen = enableSharpen;
    eyeBufferLayerSettings.expensiveSharpen = expensiveSharpen;
}

void YVRRenderConfigsMgr::setFrameVsyncCount(const int& vSyncCount) { frameEndInfoExt.minVsync = vSyncCount; }

const XrVector2f& YVRRenderConfigsMgr::getEyeBufferResolution(int renderPass)
{
    /// while not using quad views, we only care about first render pass
    if (!isUsingQuadViews())
        renderPass = 0;

    // In quad views, configs is outer left, outer right, inner left, inner right
    // thus, to get outer and inner resolution, we need to multiply renderPass by 2
    const auto targetView = isUsingQuadViews() ? plugin.openxrMgr->program->quadViewsConfigs[renderPass * 2]
                                : plugin.openxrMgr->program->stereoViewsConfigs[renderPass];

    eyeBufferResolution.x = targetView.recommendedImageRectWidth * getResolutionScale();
    eyeBufferResolution.y = targetView.recommendedImageRectHeight * getResolutionScale();

    return eyeBufferResolution;
}

void YVRRenderConfigsMgr::getRenderScale(const char* packageName, YVRRenderScaleBuffers* renderScaleBuffers)
{
    AnnounceCallingFunc();
    XrRenderScaleBuffersYVR buffers{XR_TYPE_RENDER_SCALE_BUFFERS_YVR};
    buffers.renderScaleCapacityInput = 0;
    buffers.renderScaleCountOutput = 0;
    buffers.renderScales = nullptr;

    OpenXRAPI(xrGetRenderScaleYVR(plugin.openxrMgr->program->session, packageName, &buffers));

    if (renderScaleBuffers->renderScaleCapacityInput == 0)
    {
        renderScaleBuffers->renderScaleCountOutput = buffers.renderScaleCountOutput;
        return;
    }

    std::vector<XrRenderScaleYVR> renderScales{buffers.renderScaleCountOutput};
    buffers.renderScales = renderScales.data();

    buffers.renderScaleCapacityInput = buffers.renderScaleCountOutput;
    OpenXRAPI(xrGetRenderScaleYVR(plugin.openxrMgr->program->session, packageName, &buffers));
    memcpy(renderScaleBuffers->renderScales, buffers.renderScales, sizeof(XrRenderScaleYVR) * buffers.renderScaleCountOutput);

}
void YVRRenderConfigsMgr::setRenderScale(const char* packageName, XrRenderScaleYVR renderScale)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetRenderScaleYVR(plugin.openxrMgr->program->session,packageName,renderScale));
}

void YVRRenderConfigsMgr::getRenderSharpen(const char* packageName, XrSharpenInfoYVR* sharpenInfoYVR)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetRenderSharpenYVR(plugin.openxrMgr->program->session,packageName,sharpenInfoYVR));

}
void YVRRenderConfigsMgr::setRenderSharpen(const char* packageName, XrSharpenInfoYVR* sharpenInfoYVR)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetRenderSharpenYVR(plugin.openxrMgr->program->session, packageName, sharpenInfoYVR));
}

void YVRRenderConfigsMgr::getRenderFov(const char* packageName, XrRenderFovYVR* renderFovYVR)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetRenderFovYVR(plugin.openxrMgr->program->session, packageName, renderFovYVR));
}

void YVRRenderConfigsMgr::setRenderFov(const char* packageName, XrRenderFovYVR* renderFovYVR)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetRenderFovYVR(plugin.openxrMgr->program->session, packageName, renderFovYVR));
}

void YVRRenderConfigsMgr::resetRenderFov(const char* packageName, XrFovResetTypeYVR resetType)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrResetRenderFovYVR(plugin.openxrMgr->program->session, packageName, resetType));
}

void YVRRenderConfigsMgr::getAppColorSpace(const char* packageName, XrColorSpaceConfigYVR* outColorSpace)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetColorSpaceConfigYVR(plugin.openxrMgr->program->session, packageName, outColorSpace));
}

void YVRRenderConfigsMgr::setAppColorSpace(const char* packageName, const XrColorSpaceConfigYVR* colorSpace)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetColorSpaceConfigYVR(plugin.openxrMgr->program->session, packageName, colorSpace));
}