#pragma once

#include "TaskBase.h"

class SwapLayerBufferTask : public TaskBase
{
  private:
    int layerId;

  public:
    SwapLayerBufferTask(int layerId);
    ~SwapLayerBufferTask() override;
    void runTask() override;
};
