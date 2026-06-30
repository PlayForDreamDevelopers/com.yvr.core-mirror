#pragma once

#include "..\..\XRData.h"
#include "openxr.h"
#include "openxr_yvr_private.h"

enum YVRStereoRenderMode
{
    MultiPasses,
    SinglePassMultiView = 2,
    QuadViews = 3,
};

enum YVRDepthBufferSize
{
    Bit_16,
    Bit_24
};

struct YVRLayerSettings
{
    bool enableSuperSample;
    bool expensiveSuperSample;
    bool enableSharpen;
    bool expensiveSharpen;
};

struct YVRRenderScaleBuffers
{
    uint32_t renderScaleCapacityInput;
    uint32_t renderScaleCountOutput;
    XrRenderScaleYVR* renderScales;
};


class YVRRenderConfigsMgr
{
public:
    const XrFoveationDynamicFB& getFFRDynamic() const { return foveationDynamic; }
    void setFFRDynamic(const XrFoveationDynamicFB& dynamic) { this->foveationDynamic = dynamic; }

    const XrFoveationLevelFB& getFFRLevel() const { return foveationLevel; }
    void setFFRLevel(const XrFoveationLevelFB& level) { this->foveationLevel = level; }

    const YVRStereoRenderMode& getStereoRenderMode() const { return renderMode; }
    void refreshSystemProperty(bool force = false);

    bool isUsingSinglePass() const;
    bool isUsingQuadViews() const;
    void seStereoRenderMode(const YVRStereoRenderMode& mode) { this->renderMode = mode; }

    int getViewsCount() const;

    const YVRDepthBufferSize& getDepthBufferSize() const { return depthBufferSize; }
    void setDepthBufferSize(const YVRDepthBufferSize& size) { this->depthBufferSize = size; }

    const XrColorSpaceYVR& getColorSpace() const { return colorSpace; }
    void setColorSpace(const XrColorSpaceYVR& color) { this->colorSpace = color; }

    const bool& isP3() const { return this->isP3ColorSpace; }
    void setP3ColorSpace(const bool isP3) { this->isP3ColorSpace = isP3; }

    const bool& isFullResolutionScaleModified() const { return fullResolutionScaleModified; }
    const float& getResolutionScale() const { return resolutionScale; }
    void setResolutionScale(const float& scale)
    {
        this->resolutionScale = scale;
        fullResolutionScaleModified = true;
    }

    const float& getOuterPassResolutionScale() const { return outerPassResolutionScale; }
    void setOuterPassResolutionScale(const float& scale) { this->outerPassResolutionScale = scale; }

    const float& getInnerPassResolutionScale() const { return innerPassResolutionScale; }
    void setInnerPassResolutionScale(const float& scale) { this->innerPassResolutionScale = scale; }

    const float& getOuterExtraPassResolutionScale() const { return outerExtraPassResolutionScale; }
    void setOuterExtraPassResolutionScale(const float& scale) { this->outerExtraPassResolutionScale = scale; }

    const float& getInnerExtraPassResolutionScale() const { return innerExtraPassResolutionScale; }
    void setInnerExtraPassResolutionScale(const float& scale) { this->innerExtraPassResolutionScale = scale; }

    const int& getExtraRenderPassDepth() const { return extraRenderPassDepth; }
    void setExtraRenderPassDepth(const int& depth) { this->extraRenderPassDepth = depth; }

    const YVRLayerSettings& getEyeBufferLayerSettings();
    void setEyeBufferLayerSettings(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen);

    void setFrameVsyncCount(const int& vSyncCount);

    const int& getDisplayRate();
    void setDisplayRate(const int& displayRate);

    const bool& isMonoscopic();
    void setMonoscopic(const bool& isMonoscopic);

    const bool& isAutoResolve();
    void setAutoResolve(const bool& autoResolve);

    const bool& hasExtraRenderPass();
    void setExtraRenderPass(const bool& extraRenderPass);

    YVRStereoRenderMode getActualRenderingMode();

    const bool& hasExtraLatency();
    void setExtraLatency(const bool& extraLatency);

    const bool& isAppSWEnable();
    void setAppSWEnable(const bool& enable);

    const bool& isAppSWSwitchOn();
    void setAppSWSwitch(const bool& isOn);

    const bool& optimizeBufferDiscardsEnable();
    void setOptimizeBufferDiscards(const bool& isEnable);

    const bool& isUsingNativeDepthBuffer();
    void setUseNativeDepthBuffer(const bool& useNativeDepthBuffer);

    const bool& passthroughProviderEnable();
    void setPassthroughProviderEnable(const bool& isEnable);

    bool requireAllTransLayer();

    void setSystemMenuMode(const bool& enable);
    void setPassThroughMode(const bool& enable);

    const XrVector2f& getEyeBufferResolution(int renderPass = 0);

    void setRenderViewport(const XrVector2f& renderViewport) { this->renderViewport = renderViewport; }
    const XrVector2f& getRenderViewport() const { return renderViewport; }

    void setAdapterResolutionPolicy(XrAdapterResolutionPolicyYVR policy) { this->adapterResolutionInfo.resolutionPolicy = policy; }
    const XrAdapterResolutionInfoYVR* GetAdapterResolutionInfo() { return &this->adapterResolutionInfo; }

    void getRenderScale(const char* packageName, YVRRenderScaleBuffers* renderScaleBuffers);
    void setRenderScale(const char* packageName, XrRenderScaleYVR renderScale);
    void getRenderSharpen(const char* packageName, XrSharpenInfoYVR* sharpenInfoYVR);
    void setRenderSharpen(const char* packageName, XrSharpenInfoYVR* sharpenInfoYVR);
    void getRenderFov(const char* packageName, XrRenderFovYVR* renderFovYVR);
    void setRenderFov(const char* packageName, XrRenderFovYVR* renderFovYVR);
    void resetRenderFov(const char* packageName, XrFovResetTypeYVR resetType);
    void getAppColorSpace(const char* packageName, XrColorSpaceConfigYVR* outColorSpace);
    void setAppColorSpace(const char* packageName, const XrColorSpaceConfigYVR* colorSpace);

    YVRRenderConfigsMgr();
    ~YVRRenderConfigsMgr();

    float ipd{};

    XrFrameEndInfoExtYVR frameEndInfoExt{XR_TYPE_FRAME_END_INFO_EXT_YVR};

private:
    const int defaultDisplayRate = 90.0f;
    int displayRate{-1};
    bool monoscopic{false};
    bool autoResolve{false};
    bool extraRenderPass{false};
    bool extraLatency{false};
    bool appSWEnabled{false};
    bool appSWSwitch{false};
    bool useNativeDepthBuffer{false};
    bool systemMenuMode{false};
    bool passThroughMode{false};
    bool optimizeBufferDiscards{false};
    bool enablePassthroughProvider{false};
    bool isP3ColorSpace{false};

    YVRStereoRenderMode renderMode{};
    YVRDepthBufferSize depthBufferSize{};
    XrColorSpaceYVR colorSpace{};
    XrVector2f eyeBufferResolution{};
    XrFrameEndInfoLayerTypeExtYVR frameEndInfoLayerTypeExt{XR_TYPE_FRAME_END_INFO_LAYERTYPE_EXT_YVR};
    XrFoveationLevelFB foveationLevel{};
    XrFoveationDynamicFB foveationDynamic{};
    bool fullResolutionScaleModified{};
    float resolutionScale{};
    float outerPassResolutionScale{}; // Only works for quad views
    float innerPassResolutionScale{}; // Only works for quad views
    float outerExtraPassResolutionScale{}; //Only works for quadviews and extraRenderPass enabled
    float innerExtraPassResolutionScale{}; //Only works for quadviews and extraRenderPass enabled
    int extraRenderPassDepth{}; //Only works if extraRenderPass enable
    XrVector2f renderViewport{};
    YVRLayerSettings eyeBufferLayerSettings{false, false, false, false};
    XrAdapterResolutionInfoYVR adapterResolutionInfo{XR_TYPE_ADAPTER_RESOLUTIONINFO_YVR};
};
