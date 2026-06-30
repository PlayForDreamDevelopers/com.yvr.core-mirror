#include "SwapLayerBufferTask.h"
#include "../../../YVRUnityPlugin.h"

SwapLayerBufferTask::SwapLayerBufferTask(int layerId) : TaskBase(), layerId(layerId) {}

SwapLayerBufferTask::~SwapLayerBufferTask() = default;

void SwapLayerBufferTask::runTask() { plugin.renderMgr->renderLayersMgr->getRenderLayer(layerId)->swapChain(); }