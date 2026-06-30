#pragma once
#include "../YVRRenderLayer.h"
#include "TaskBase.h"

typedef void (*OnPrepareHandleLayerCallback)(void* layerContainer);

class PrepareHandleLayerTask : public TaskBase
{
  private:
    void* layerContainer;
    OnPrepareHandleLayerCallback onPrepareHandleLayerCallback;

  public:
    PrepareHandleLayerTask(void* layerContianer, OnPrepareHandleLayerCallback onPrepareLayerCallback);
    ~PrepareHandleLayerTask() override;
    void runTask() override;
};
