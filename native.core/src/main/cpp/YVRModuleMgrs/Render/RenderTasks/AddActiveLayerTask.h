#pragma once
#include "TaskBase.h"

typedef void (*OnLayerAddedCallback)(void* layerContainer);

class AddActiveLayerTask : public TaskBase
{
  private:
    int layerId;
    void* layerContainer;
    OnLayerAddedCallback onLayerAddedCallback;

  public:
    AddActiveLayerTask(int layerId, void* layerContainer, OnLayerAddedCallback onLayerAddedCallback);
    ~AddActiveLayerTask() override;
    void runTask() override;
};
