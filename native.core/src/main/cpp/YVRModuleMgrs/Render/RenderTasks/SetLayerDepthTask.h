#pragma once
#include "TaskBase.h"

typedef void (*OnLayerDepthChangedCallback)(void* layerContainer, bool result);

class SetLayerDepthTask : public TaskBase
{
  private:
    int layerId;
    int newDepth;
    void* layerContainer;
    OnLayerDepthChangedCallback onLayerDepthChangedCallback;

  public:
    SetLayerDepthTask(int layerId, int depth, void* layerContainer, OnLayerDepthChangedCallback onSetLayerDepthCallback);
    ~SetLayerDepthTask() override;
    void runTask() override;
};