#pragma once

#include "YVRRenderConfigsMgr.h"
#include "YVRRenderLayersMgr.h"
#include "YVRRenderSwapchainMgr.h"

#include "openxr/openxr.h"

class YVRLayer;
struct yvrTextureSwapChain;

class YVRRenderingMgr
{
public:
    YVRRenderLayersMgr* renderLayersMgr;
    YVRRenderConfigsMgr* configsMgr;
    YVRRenderSwapchainMgr* renderSwapChainMgr;

    YVRRenderingMgr();
    ~YVRRenderingMgr();

    void submitFrame();
    void skipFrame();

    void setSkipFrameCount(int frameCount);
    bool shouldSkipFrame();

    void WaitFrame();
    void BeginFrame();
    void EndFrame();

    XrFrameEndInfo frameEndInfo{XR_TYPE_FRAME_END_INFO};
    XrFrameState frameState{XR_TYPE_FRAME_STATE};
    bool frameBegan = false;
    bool frameWaited = false;

private:
    int skipFrameCount;
};
