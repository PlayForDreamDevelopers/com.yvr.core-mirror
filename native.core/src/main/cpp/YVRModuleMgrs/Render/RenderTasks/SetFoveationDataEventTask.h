#pragma once
#include "TaskBase.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"

class SetFoveationDataEventTask : public TaskBase
{
private:
    XrFoveationLevelFB level;
    XrFoveationDynamicFB dynamic;
public:
    SetFoveationDataEventTask(XrFoveationLevelFB level, XrFoveationDynamicFB dynamic);
    ~SetFoveationDataEventTask() override;
    void runTask() override;
};
