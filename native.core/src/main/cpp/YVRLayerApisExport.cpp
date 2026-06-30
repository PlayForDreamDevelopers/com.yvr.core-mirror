#include "YVRModuleMgrs/Render/RenderTasks/AddActiveLayerTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/CreateRenderLayerTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/DestroyRenderLayerTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/RemoveActiveLayerTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/PrepareHandleLayerTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/SwapLayerBufferTask.h"
#include "YVRModuleMgrs/Render/RenderTasks/SetEyeBufferSettingsTask.h"

#include "YVRModuleMgrs/Render/RenderTasks/TriggerLayerPreSubmitCallbackTask.h"

#include "YVRModuleMgrs/Render/RenderTasks/SetLayerDepthTask.h"
#include "YVRUnityPlugin.h"

extern "C"
{
void yvrCreateRenderLayerAsync(LayerCreateInfo layerCreateInfo, void* layerContainer, OnLayerCreatedCallback onLayerCreatedCallback,
                               bool showAfterCreated = true)
{
    plugin.gfxTasksMgr->PushOnPostSubmitTask(new CreateRenderLayerTask(layerCreateInfo, layerContainer, onLayerCreatedCallback));
}

void yvrDestroyRenderLayerAsync(int layerId, bool immediate, void* layerContainer, OnLayerDeletedCallback onLayerDeletedCallback)
{
    plugin.gfxTasksMgr->PushOnPostSubmitTask(new DestroyRenderLayerTask(layerId, immediate, layerContainer, onLayerDeletedCallback));
}

void yvrAddActiveLayerAsync(int layerId, void* layerContainer, OnLayerAddedCallback onLayerAddedCallback)
{
    plugin.gfxTasksMgr->PushOnPostSubmitTask(new AddActiveLayerTask(layerId, layerContainer, onLayerAddedCallback));
}

void yvrRemoveActiveLayerAsync(int layerId, void* layerContainer, OnLayerRemovedCallback onLayerRemovedCallback)
{
    plugin.gfxTasksMgr->PushOnPostSubmitTask(new RemoveActiveLayerTask(layerId, layerContainer, onLayerRemovedCallback));
}

void yvrPrepareHandleLayerAsync(void* layerContianer, OnPrepareHandleLayerCallback onPrepareHandleLayerCallback)
{
    plugin.gfxTasksMgr->PushOnPostSubmitTask(new PrepareHandleLayerTask(layerContianer, onPrepareHandleLayerCallback));
}

int yvrCreateRenderLayer(LayerCreateInfo layerCreateInfo)
{
    return plugin.renderMgr->renderLayersMgr->createLayer(layerCreateInfo);
}

void yvrDestroyRenderLayer(int layerId, bool destroyImmediate)
{
    plugin.renderMgr->renderLayersMgr->deleteRenderLayer(layerId, destroyImmediate);
}

void yvrAddActiveLayer(int layerId)
{
    plugin.renderMgr->renderLayersMgr->addActiveLayer(layerId);
}

void yvrRemoveActiveLayer(int layerId)
{
    plugin.renderMgr->renderLayersMgr->removeActiveLayer(layerId);
}

void yvrSetLayerVisible(int layerId, bool visible)
{
    plugin.renderMgr->renderLayersMgr->setLayerVisible(layerId, visible);
}

void yvrSetLayerPose(int layerId, XrPosef pose)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setPose(pose);
}

void yvrSetLayerSize(int layerId, XrExtent2Df size)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setSize(size);
}

void yvrSetCylinderParam(int layerId, float radius, float centralAngle, float aspectRatio)
{
    reinterpret_cast<YVRCylinderRenderLayer*>(plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId))->setCylinderParam(radius, centralAngle, aspectRatio);
}

void yvrSetEquirectRadius(int layerId, float radius)
{
    reinterpret_cast<YVREquirectRenderLayer*>(plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId))->setEquirectRadius(radius);
}

void yvrSetEquirectOffset(int layerId, yvrRect offset)
{
    reinterpret_cast<YVREquirectRenderLayer*>(plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId))->setEquirectOffset(offset);
}

void yvrSetEquirect2Param(int layerId, float radius, float centralHorizontalAngle, float upperVerticalAngle, float lowerVerticalAngle)
{
    reinterpret_cast<YVREquirect2RenderLayer*>(plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId))->setEquirect2Param(radius, centralHorizontalAngle, upperVerticalAngle, lowerVerticalAngle);
}

void yvrSetEyeBufferSettings(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen)
{
    plugin.gfxTasksMgr->PushOnPreSubmitTask(new SetEyeBufferSettingsTask(enableSuperSample, expensiveSuperSample, enableSharpen, expensiveSharpen));
}

void yvrSetLayerSettings(int layerId, bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setLayerSettings(enableSuperSample, expensiveSuperSample, enableSharpen, expensiveSharpen);
}

void yvrSetLayerImageRect(int layerId, yvrRect rect)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setImageRect(rect);
}

void yvrSetLayerDimensions(int layerId, int width, int height)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setSurfaceSwapChainDimensions(width, height);
}

void yvrSwapLayerBuffer(int layerId) { plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->swapChain(); }

int yvrGetLayersCount() { return plugin.renderMgr->renderLayersMgr->getLayersCount(); }

void yvrGetAllLayersColorHandle(int* layersColorHandle) { plugin.renderMgr->renderLayersMgr->getAllLayersColorHandle(layersColorHandle); }

int yvrGetEyeBufferLayerId() { return plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->id; }

jobject yvrGetLayerSurface(int layerId) { return plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->GetSurface(); }

int yvrGetLayerColorHandle(int layerId,xrEyeSide eyeSide, int index)
{
    return plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->getColorHandle(eyeSide, index);
}

void yvrSetLayerPreSubmitCallback(void* layerContainer, OnLayerPreSubmitCallback onLayerPreSubmitCallback)
{
    plugin.gfxTasksMgr->PushOnPreSubmitTask(new TriggerLayerPreSubmitCallbackTask(layerContainer, onLayerPreSubmitCallback));
}

void yvrSetLayerDepthAsync(int layerId, int newDepth, void* layerContainer, OnLayerDepthChangedCallback onLayerDepthChangedCallback)
{
    AnnounceCallingFunc();
    plugin.gfxTasksMgr->PushOnPrepareTask(new SetLayerDepthTask(layerId, newDepth, layerContainer, onLayerDepthChangedCallback));
}

void yvrSetLayerSupportDPComposition(int layerId, bool isSupport)
{
    plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->setSupportDPComposition(isSupport);
}
}
