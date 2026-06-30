#include "AddActiveLayerTask.h"
#include "../../../YVRUnityPlugin.h"

AddActiveLayerTask::AddActiveLayerTask(int layerId, void* layerContainer, OnLayerAddedCallback onLayerAddedCallback)
    : TaskBase(), layerId(layerId), layerContainer(layerContainer), onLayerAddedCallback(onLayerAddedCallback)
{
}

AddActiveLayerTask::~AddActiveLayerTask() = default;

void AddActiveLayerTask::runTask()
{
    plugin.renderMgr->renderLayersMgr->addActiveLayer(this->layerId);
    if (onLayerAddedCallback != nullptr) onLayerAddedCallback(layerContainer);
}