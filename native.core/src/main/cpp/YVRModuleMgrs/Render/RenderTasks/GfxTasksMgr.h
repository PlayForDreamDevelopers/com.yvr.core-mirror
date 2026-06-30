#pragma once
#include "TaskBase.h"
#include "TaskQueue.h"

class GfxTasksMgr
{
public:
    GfxTasksMgr();
    ~GfxTasksMgr();

    void PushOnPreSubmitTask(TaskBase* task);
    void PushOnPostSubmitTask(TaskBase* task);
    void PushOnPrepareTask(TaskBase* task);

    bool HandleAnOnPreSubmitTask();
    bool HandleAnOnPostSubmitTask();
    bool HandleAnOnPrepareTask();

private:
    TaskQueue<TaskBase*> onPreSubmitTaskQueue;
    TaskQueue<TaskBase*> onPrepareTaskQueue;
    TaskQueue<TaskBase*> onPostSubmitTaskQueue;
};
