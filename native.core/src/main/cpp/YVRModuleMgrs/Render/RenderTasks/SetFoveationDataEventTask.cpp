#include "SetFoveationDataEventTask.h"
#include "../../../YVRUnityPlugin.h"

SetFoveationDataEventTask::SetFoveationDataEventTask(XrFoveationLevelFB level, XrFoveationDynamicFB dynamic)
    : TaskBase(),
      level(level),
      dynamic(dynamic)
{

}

SetFoveationDataEventTask::~SetFoveationDataEventTask() = default;

void SetFoveationDataEventTask::runTask()
{
    plugin.renderMgr->configsMgr->setFFRLevel(level);
    plugin.renderMgr->configsMgr->setFFRDynamic(dynamic);
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->setFoveationLevel(level, dynamic);
}
