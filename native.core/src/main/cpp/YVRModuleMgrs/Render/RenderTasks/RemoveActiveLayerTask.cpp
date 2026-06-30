#include "RemoveActiveLayerTask.h"
#include "../../../YVRUnityPlugin.h"

RemoveActiveLayerTask::RemoveActiveLayerTask(int layerId, void* layerContainer, OnLayerRemovedCallback onLayerRemovedCallback)
    : TaskBase(), layerId(layerId), layerContainer(layerContainer), onLayerRemovedCallback(onLayerRemovedCallback)
{
}

RemoveActiveLayerTask::~RemoveActiveLayerTask() = default;

void RemoveActiveLayerTask::runTask()
{
    plugin.renderMgr->renderLayersMgr->removeActiveLayer(this->layerId);
    if (onLayerRemovedCallback != nullptr) onLayerRemovedCallback(layerContainer);
}