#pragma once

#include "TaskBase.h"

typedef void (*OnLayerPreSubmitCallback)(void* layerContainer);

class TriggerLayerPreSubmitCallbackTask : public TaskBase
{
  private:
    void* layerContainer;
    OnLayerPreSubmitCallback onLayerUpdateCallback;
  public:
    TriggerLayerPreSubmitCallbackTask(void* layerContainer, OnLayerPreSubmitCallback onLayerUpdateCallback);
    ~TriggerLayerPreSubmitCallbackTask() override;
    void runTask() override;
};
