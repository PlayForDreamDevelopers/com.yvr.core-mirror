#pragma once

#include "TaskBase.h"

typedef void (*OnLayerDeletedCallback)(void* layerContainer);

class DestroyRenderLayerTask : public TaskBase
{
  private:
    int layerId;
    bool destroyImmediate;
    void* layerContainers;
    OnLayerDeletedCallback onLayerDeletedCallback;

  public:
    DestroyRenderLayerTask(int id, bool immediate, void* layerContainer, OnLayerDeletedCallback onLayerDeletedCallback);
    ~DestroyRenderLayerTask() override;
    void runTask() override;
};
