
#include "CreateRenderLayerTask.h"
#include "../../../YVRUnityPlugin.h"

CreateRenderLayerTask::CreateRenderLayerTask(LayerCreateInfo layerCreateInfo, void* layerContainer, OnLayerCreatedCallback onLayerCreateCallback)
    : layerCreateInfo(layerCreateInfo), layerContainer(layerContainer), onLayerCreatedCallback(onLayerCreateCallback)
{
}

void CreateRenderLayerTask::runTask()
{
    const int id = plugin.renderMgr->renderLayersMgr->createLayer(layerCreateInfo);

    if (onLayerCreatedCallback != nullptr) onLayerCreatedCallback(layerContainer, id);
}

CreateRenderLayerTask::~CreateRenderLayerTask() = default;
