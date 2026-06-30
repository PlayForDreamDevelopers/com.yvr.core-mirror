#include "SetEyeBufferSettingsTask.h"
#include "../../../YVRUnityPlugin.h"

SetEyeBufferSettingsTask::SetEyeBufferSettingsTask(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen)
    : TaskBase(),
      enableSuperSample(enableSuperSample),
      expensiveSuperSample(expensiveSuperSample),
      enableSharpen(enableSharpen),
      expensiveSharpen(expensiveSharpen)
{
	
}

SetEyeBufferSettingsTask::~SetEyeBufferSettingsTask() = default;

void SetEyeBufferSettingsTask::runTask()
{
    plugin.renderMgr->configsMgr->setEyeBufferLayerSettings(enableSuperSample, expensiveSuperSample, enableSharpen, expensiveSharpen);
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->setLayerSettings(enableSuperSample, expensiveSuperSample, enableSharpen, expensiveSharpen);
}