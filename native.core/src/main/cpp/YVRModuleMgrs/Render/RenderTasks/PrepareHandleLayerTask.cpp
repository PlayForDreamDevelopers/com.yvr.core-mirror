#include "PrepareHandleLayerTask.h"
#include "../../../YVRUnityPlugin.h"

PrepareHandleLayerTask::PrepareHandleLayerTask(void* layerContainer, OnPrepareHandleLayerCallback onPrepareHandleLayerCallback)
    : layerContainer(layerContainer), onPrepareHandleLayerCallback(onPrepareHandleLayerCallback)
{
}

void PrepareHandleLayerTask::runTask()
{ 
    if (onPrepareHandleLayerCallback != nullptr) onPrepareHandleLayerCallback(layerContainer);
}

PrepareHandleLayerTask::~PrepareHandleLayerTask() = default;