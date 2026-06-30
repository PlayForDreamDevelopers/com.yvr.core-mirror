#pragma once
#include "../YVRRenderLayer.h"
#include "TaskBase.h"

typedef void (*OnLayerCreatedCallback)(void* layerContainer, int layerID);

class CreateRenderLayerTask : public TaskBase
{
  private:
    LayerCreateInfo layerCreateInfo;
    void* layerContainer;
    OnLayerCreatedCallback onLayerCreatedCallback;

  public:
    CreateRenderLayerTask(LayerCreateInfo layerCreateInfo, void* layerContainer, OnLayerCreatedCallback onLayerCreateCallback);
    ~CreateRenderLayerTask() override;
    void runTask() override;
};
