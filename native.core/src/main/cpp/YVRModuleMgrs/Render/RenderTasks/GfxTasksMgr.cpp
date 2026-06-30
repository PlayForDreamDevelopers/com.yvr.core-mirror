#include "GfxTasksMgr.h"
#include <algorithm>
#include "AddActiveLayerTask.h"
#include "SetFoveationDataEventTask.h"

GfxTasksMgr::GfxTasksMgr() : onPreSubmitTaskQueue(), onPrepareTaskQueue() {}

GfxTasksMgr::~GfxTasksMgr() {}

void GfxTasksMgr::PushOnPrepareTask(TaskBase* task) { onPrepareTaskQueue.Push(task); }

void GfxTasksMgr::PushOnPreSubmitTask(TaskBase* task) { onPreSubmitTaskQueue.Push(task); }
void GfxTasksMgr::PushOnPostSubmitTask(TaskBase* task) { onPostSubmitTaskQueue.Push(task); }

bool GfxTasksMgr::HandleAnOnPreSubmitTask()
{
    TaskBase* task;
    if (onPreSubmitTaskQueue.Pop(task))
    {
        task->runTask();
        delete task;
        return true;
    }
    return false;
}

bool GfxTasksMgr::HandleAnOnPostSubmitTask()
{
    TaskBase* task;
    if (onPostSubmitTaskQueue.Pop(task))
    {
        task->runTask();
        delete task;
        return true;
    }

    return false;
}

bool GfxTasksMgr::HandleAnOnPrepareTask()
{
    TaskBase* task;
    if (onPrepareTaskQueue.Pop(task))
    {
        task->runTask();
        delete task;
        return true;
    }

    return false;
}