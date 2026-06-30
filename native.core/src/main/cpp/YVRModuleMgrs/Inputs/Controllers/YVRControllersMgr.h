#pragma once

#include "./ControllerXRActionContext.h"
#include "../XRMotionData.h"

class YVRControllersMgr
{
public:
    ControllerXRActionContext actionCtx{};
    std::array<XrSpace, COUNT> controllerSpace{};
    XRMotionData leftRenderPose{}, rightRenderPose{};
    XRMotionData leftPhysicsPose{}, rightPhysicsPose{};

    void createActions();
    void destroyActions();

    void syncControllerChargeState();
    void refreshControllerPose(const bool forRender);

    XrControllerYVR getPrimaryController();
    void setPrimaryController(const XrControllerYVR& controller);

    void setControllerVibration(bool isLeftController, float amplitude, float frequency = -1, XrDuration duration = -1);
    void stopControllerVibration(bool isLeftController);

    bool isControllerEnable = true;
    void setControllerEnable(bool enabled);

    bool getBooleanActionValue(bool isLeftController, const XrAction& action);
    float getFloatActionValue(bool isLeftController, const XrAction& action);
    bool getActiveValue(const ControllerSide side);

private:
    void createControllerAction(XrAction& action, const XrActionType type, const char* actionName, const char* localizedActionName);
    void destroyControllerAction(const XrAction& action);
};
