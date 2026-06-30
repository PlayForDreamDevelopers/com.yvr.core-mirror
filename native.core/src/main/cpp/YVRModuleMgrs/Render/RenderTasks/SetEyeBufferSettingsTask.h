#pragma once

#include "../YVRRenderLayer.h"
#include "TaskBase.h"

class SetEyeBufferSettingsTask : public TaskBase
{
  private:
    bool enableSuperSample;
    bool expensiveSuperSample;
    bool enableSharpen;
    bool expensiveSharpen;
  public:
    SetEyeBufferSettingsTask(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen);
    ~SetEyeBufferSettingsTask() override;
    void runTask() override;
};
