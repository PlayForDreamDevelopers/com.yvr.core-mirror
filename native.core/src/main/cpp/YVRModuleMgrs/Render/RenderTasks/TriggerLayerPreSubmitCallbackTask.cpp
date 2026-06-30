#include "TriggerLayerPreSubmitCallbackTask.h"

TriggerLayerPreSubmitCallbackTask::TriggerLayerPreSubmitCallbackTask(void* layerContainer, OnLayerPreSubmitCallback onLayerUpdateCallback)
    : TaskBase(), layerContainer(layerContainer), onLayerUpdateCallback(onLayerUpdateCallback)
{
}

TriggerLayerPreSubmitCallbackTask::~TriggerLayerPreSubmitCallbackTask() = default;

void TriggerLayerPreSubmitCallbackTask::runTask() { onLayerUpdateCallback(layerContainer); }
