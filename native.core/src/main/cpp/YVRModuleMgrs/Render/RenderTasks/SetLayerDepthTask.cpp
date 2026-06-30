#include "SetLayerDepthTask.h"
#include "../../../YVRUnityPlugin.h"

SetLayerDepthTask::SetLayerDepthTask(int layerId, int depth, void* layerContainer, OnLayerDepthChangedCallback onLayerDepthChangedCallback)
    : TaskBase(), layerId(layerId), newDepth(depth), layerContainer(layerContainer), onLayerDepthChangedCallback(onLayerDepthChangedCallback)
{
}

SetLayerDepthTask::~SetLayerDepthTask() = default;

void SetLayerDepthTask::runTask()
{

    bool result = plugin.renderMgr->renderLayersMgr->SetLayerDepth(this->layerId, this->newDepth);

    if (onLayerDepthChangedCallback != nullptr) onLayerDepthChangedCallback(layerContainer, result);
}