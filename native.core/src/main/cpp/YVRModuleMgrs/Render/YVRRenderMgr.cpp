#include "YVRRenderMgr.h"
#include "../../YVRUnityPlugin.h"

YVRRenderingMgr::YVRRenderingMgr()
    : skipFrameCount(0)
{
    renderLayersMgr = new YVRRenderLayersMgr();
    configsMgr = new YVRRenderConfigsMgr();
    renderSwapChainMgr = new YVRRenderSwapchainMgr();
}

YVRRenderingMgr::~YVRRenderingMgr()
{
    delete renderLayersMgr;
    delete configsMgr;
    delete renderSwapChainMgr;
}

void YVRRenderingMgr::submitFrame()
{
    if (!plugin.lifecycleMgr->isVisible())
    {
        skipFrame();
        return;
    }

    renderLayersMgr->tickAllLayers();

    const bool transFirstLayer = plugin.renderMgr->configsMgr->requireAllTransLayer();
    frameEndInfo.layers = renderLayersMgr->GetSortedXRLayers(frameEndInfo.layerCount, transFirstLayer);
    frameEndInfo.next = &plugin.renderMgr->configsMgr->frameEndInfoExt;
    EndFrame();
}

void YVRRenderingMgr::setSkipFrameCount(int frameCount) { skipFrameCount = frameCount; }

void YVRRenderingMgr::skipFrame()
{
    renderLayersMgr->tickAllLayers();

    frameEndInfo.layerCount = 0;
    frameEndInfo.layers = nullptr;

    if (plugin.lifecycleMgr->isInVrMode())
    {
        EndFrame();
    }

    skipFrameCount--;
}

bool YVRRenderingMgr::shouldSkipFrame() { return skipFrameCount > 0 || plugin.lifecycleMgr->getSubmittedFrameCount() == 0; }


void YVRRenderingMgr::BeginFrame()
{
    if (!plugin.lifecycleMgr->isInVrMode() || frameBegan || !frameWaited) { return; }

    XrFrameBeginInfo frameBeginInfo{XR_TYPE_FRAME_BEGIN_INFO};
    OpenXRAPI(xrBeginFrame(plugin.openxrMgr->program->session,&frameBeginInfo));
    frameBegan = true;
}


void YVRRenderingMgr::WaitFrame()
{
    if (!plugin.lifecycleMgr->isInVrMode() || frameWaited) { return; }

    XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
    OpenXRAPI(xrWaitFrame(plugin.openxrMgr->program->session, &frameWaitInfo, &frameState));
    frameWaited = true;
}

void YVRRenderingMgr::EndFrame()
{
    if (!plugin.lifecycleMgr->isInVrMode() || !frameBegan) { return; }

    frameEndInfo.displayTime = frameState.predictedDisplayTime;
    frameEndInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;

    OpenXRAPI(xrEndFrame(plugin.openxrMgr->program->session, &frameEndInfo));
    frameBegan = false;
    frameWaited = false;
}
