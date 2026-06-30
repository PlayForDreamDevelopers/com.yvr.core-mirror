#pragma once

#include "../UnityHeader/XR/IUnityXRDisplay.h"
#include "../YVRUnityPlugin.h"
#include "YVRProviderBase.h"

#include <array>

class YVRDisplayProvider : YVRProviderBase
{
  public:
    YVRDisplayProvider(YVRUnityXRProviderMgr &context, UnitySubsystemHandle handle);
    ~YVRDisplayProvider();

    void DestroyUnityTexture();

    UnitySubsystemErrorCode initialize() override;
    UnitySubsystemErrorCode start() override;
    void stop() override;
    void shutDown() override;

  private:
    std::array<UnityXRRenderTextureDesc, EYE_NUM> unityTextureDescArray;
    std::array<UnityXRRenderTextureId, EYE_NUM * SWAP_BUFFER_NUM> unityTextureIDArray;
    std::array<UnityXRRenderTextureDesc, EYE_NUM> unityEnvironmentTextureDescArray;
    std::array<UnityXRRenderTextureId, EYE_NUM * SWAP_BUFFER_NUM> unityEnvironmentTextureIDArray;
    std::array<UnityXRRenderTextureDesc, EYE_NUM> unityMotionVectorTextureDescArray;
    std::array<UnityXRRenderTextureId, EYE_NUM * SWAP_BUFFER_NUM> unityMotionVectorTextureIDArray;

    void initializeUnityGfxThreadLifecycle(UnityXRDisplayGraphicsThreadProvider &unityGfxThreadProvider);
    void initializeRenderLayer();
    void initializeUnityTexture();
    void destroyTextures();
    void swapUnityRenderBuffer(UnityXRNextFrameDesc *nextFrameDesc);
    void updateRenderPasses(UnityXRNextFrameDesc *nextFrameDesc);
    void updateCullPasses(UnityXRNextFrameDesc *nextFrameDesc);

    UnitySubsystemErrorCode onGfxThreadStart(UnityXRRenderingCapabilities *renderingCaps);
    UnitySubsystemErrorCode onGfxThreadSubmitCurrentFrame();
    UnitySubsystemErrorCode onGfxThreadPopulateNextFrameDesc(const UnityXRFrameSetupHints *frameHints, UnityXRNextFrameDesc *nextFrame);
    UnitySubsystemErrorCode onGfxThreadStop();

    bool startNeverCalled = true;

    const UnityProfilerMarkerDesc* populateNextFrameDescMarker = nullptr;
    const UnityProfilerMarkerDesc* submitCurrentFrameMarker = nullptr;
};
