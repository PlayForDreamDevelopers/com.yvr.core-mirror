#pragma once
#include "TaskBase.h"

typedef void (*OnLayerRemovedCallback)(void* layerContainer);

class RemoveActiveLayerTask : public TaskBase
{
  private:
    int layerId;
    void* layerContainer;
    OnLayerRemovedCallback onLayerRemovedCallback;

  public:
    RemoveActiveLayerTask(int layerId, void* layerContainer, OnLayerRemovedCallback onLayerRemovedCallback);
    ~RemoveActiveLayerTask() override;
    void runTask() override;
};
