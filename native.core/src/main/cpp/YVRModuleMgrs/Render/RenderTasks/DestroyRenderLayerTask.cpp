#include "DestroyRenderLayerTask.h"
#include "../../../YVRUnityPlugin.h"

DestroyRenderLayerTask::DestroyRenderLayerTask(int id, bool immediate, void* layerContainer, OnLayerDeletedCallback onLayerDeletedCallback)
    : TaskBase(), layerId(id), destroyImmediate(immediate), layerContainers(layerContainer), onLayerDeletedCallback(onLayerDeletedCallback)
{
}

DestroyRenderLayerTask::~DestroyRenderLayerTask() = default;

void DestroyRenderLayerTask::runTask()
{
    plugin.renderMgr->renderLayersMgr->deleteRenderLayer(layerId, destroyImmediate);
    if (onLayerDeletedCallback != nullptr) onLayerDeletedCallback(layerContainers);
}