#pragma once

class TaskBase
{
  public:
    TaskBase();
    virtual void runTask() = 0;

    virtual ~TaskBase();
};