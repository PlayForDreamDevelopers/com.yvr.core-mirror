#include "YVRControllersMgr.h"
#include "../../../YVRUnityPlugin.h"

void YVRControllersMgr::createActions()
{
    const XrSession& session = plugin.openxrMgr->program->session;
    const XrInstance& instance = plugin.openxrMgr->program->instance;
    XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
    strcpy(actionSetInfo.actionSetName, "controller");
    strcpy(actionSetInfo.localizedActionSetName, "Controller");
    actionSetInfo.priority = 0;
    OpenXRAPI(xrCreateActionSet(instance, &actionSetInfo, &actionCtx.actionSet));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left", &actionCtx.controllerSubActionPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right", &actionCtx.controllerSubActionPath[ControllerSide::RIGHT]));

    createControllerAction(actionCtx.poseAction, XR_ACTION_TYPE_POSE_INPUT, "hand_pose", "Hand Pose");
    createControllerAction(actionCtx.vibrateAction, XR_ACTION_TYPE_VIBRATION_OUTPUT, "vibrate_hand", "Vibrate Hand");

    createControllerAction(actionCtx.primaryButtonAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "primary_button", "Primary Button");
    createControllerAction(actionCtx.primaryTouchAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "primary_touch", "Primary Touch");
    createControllerAction(actionCtx.secondaryButtonAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "secondary_button", "Secondary Button");
    createControllerAction(actionCtx.secondaryTouchAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "secondary_touch", "Secondary Touch");

    createControllerAction(actionCtx.thumbstickButtonAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "thumbstick_button", "Thumbstick Button");
    createControllerAction(actionCtx.thumbstickTouchAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "thumbstick_touch", "Thumbstick Touch");

    createControllerAction(actionCtx.gripButtonAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "grip_button", "Grip Button");
    createControllerAction(actionCtx.menuButtonAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "menu_button", "Menu Button");

    createControllerAction(actionCtx.triggerValue, XR_ACTION_TYPE_FLOAT_INPUT, "trigger_value", "Trigger");
    createControllerAction(actionCtx.triggerTouchAction, XR_ACTION_TYPE_BOOLEAN_INPUT, "trigger_touch", "Trigger Touch");

    createControllerAction(actionCtx.thumbstickXValue, XR_ACTION_TYPE_FLOAT_INPUT, "thumbstick_x_value", "Thumbstick X Value");
    createControllerAction(actionCtx.thumbstickYValue, XR_ACTION_TYPE_FLOAT_INPUT, "thumbstick_y_value", "Thumbstick Y Value");

    std::array<XrPath, COUNT> posePath;
    std::array<XrPath, COUNT> hapticPath;

    std::array<XrPath, COUNT> primaryButtonPath;
    std::array<XrPath, COUNT> primaryTouchPath;
    std::array<XrPath, COUNT> secondaryButtonPath;
    std::array<XrPath, COUNT> secondaryTouchPath;
    std::array<XrPath, COUNT> gripButtonPath;
    std::array<XrPath, COUNT> menuButtonPath;
    std::array<XrPath, COUNT> thumbstickButtonPath;
    std::array<XrPath, COUNT> thumbstickTouchPath;

    std::array<XrPath, COUNT> triggerPath;
    std::array<XrPath, COUNT> triggerTouchPath;

    std::array<XrPath, COUNT> thumbstickXValuePath;
    std::array<XrPath, COUNT> thumbstickYValuePath;

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/grip/pose", &posePath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/grip/pose", &posePath[ControllerSide::RIGHT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/output/haptic", &hapticPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/output/haptic", &hapticPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/x/click", &primaryButtonPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/x/touch", &primaryTouchPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/a/click", &primaryButtonPath[ControllerSide::RIGHT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/a/touch", &primaryTouchPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/y/click", &secondaryButtonPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/y/touch", &secondaryTouchPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/b/click", &secondaryButtonPath[ControllerSide::RIGHT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/b/touch", &secondaryTouchPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/y/click", &secondaryButtonPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/y/touch", &secondaryTouchPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/b/click", &secondaryButtonPath[ControllerSide::RIGHT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/b/touch", &secondaryTouchPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/trigger/value", &triggerPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/trigger/value", &triggerPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/trigger/touch", &triggerTouchPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/trigger/touch", &triggerTouchPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/squeeze/click", &gripButtonPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/squeeze/click", &gripButtonPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/menu/click", &menuButtonPath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/system/click", &menuButtonPath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/thumbstick/click", &thumbstickButtonPath[ControllerSide::LEFT]))
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/thumbstick/click", &thumbstickButtonPath[ControllerSide::RIGHT]))

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/thumbstick/touch", &thumbstickTouchPath[ControllerSide::LEFT]))
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/thumbstick/touch", &thumbstickTouchPath[ControllerSide::RIGHT]))

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/thumbstick/x", &thumbstickXValuePath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/thumbstick/x", &thumbstickXValuePath[ControllerSide::RIGHT]));

    OpenXRAPI(xrStringToPath(instance, "/user/hand/left/input/thumbstick/y", &thumbstickYValuePath[ControllerSide::LEFT]));
    OpenXRAPI(xrStringToPath(instance, "/user/hand/right/input/thumbstick/y", &thumbstickYValuePath[ControllerSide::RIGHT]));

    XrPath yvrInteractionProfilePath;
    OpenXRAPI(xrStringToPath(instance, "/interaction_profiles/yvr/touch_controller_yvr", &yvrInteractionProfilePath));

    const std::vector<XrActionSuggestedBinding> bindings{{
            {actionCtx.poseAction, posePath[LEFT]},
            {actionCtx.poseAction, posePath[RIGHT]},
            {actionCtx.vibrateAction, hapticPath[LEFT]},
            {actionCtx.vibrateAction, hapticPath[RIGHT]},
            {actionCtx.primaryButtonAction, primaryButtonPath[LEFT]},
            {actionCtx.primaryButtonAction, primaryButtonPath[RIGHT]},
            {actionCtx.secondaryButtonAction, secondaryButtonPath[LEFT]},
            {actionCtx.secondaryButtonAction, secondaryButtonPath[RIGHT]},
            {actionCtx.primaryTouchAction, primaryTouchPath[LEFT]},
            {actionCtx.primaryTouchAction, primaryTouchPath[RIGHT]},
            {actionCtx.secondaryTouchAction, secondaryTouchPath[LEFT]},
            {actionCtx.secondaryTouchAction, secondaryTouchPath[RIGHT]},
            {actionCtx.gripButtonAction, gripButtonPath[LEFT]},
            {actionCtx.gripButtonAction, gripButtonPath[RIGHT]},
            {actionCtx.menuButtonAction, menuButtonPath[LEFT]},
            {actionCtx.menuButtonAction, menuButtonPath[RIGHT]},
            {actionCtx.thumbstickButtonAction, thumbstickButtonPath[LEFT]},
            {actionCtx.thumbstickButtonAction, thumbstickButtonPath[RIGHT]},
            {actionCtx.thumbstickTouchAction, thumbstickTouchPath[LEFT]},
            {actionCtx.thumbstickTouchAction, thumbstickTouchPath[RIGHT]},
            {actionCtx.thumbstickXValue, thumbstickXValuePath[LEFT]},
            {actionCtx.thumbstickXValue, thumbstickXValuePath[RIGHT]},
            {actionCtx.thumbstickYValue, thumbstickYValuePath[LEFT]},
            {actionCtx.thumbstickYValue, thumbstickYValuePath[RIGHT]},
            {actionCtx.triggerValue, triggerPath[LEFT]},
            {actionCtx.triggerValue, triggerPath[RIGHT]},
            {actionCtx.triggerTouchAction, triggerTouchPath[LEFT]},
            {actionCtx.triggerTouchAction, triggerTouchPath[RIGHT]}
        }};

    XrInteractionProfileSuggestedBinding suggestedBindings{XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
    suggestedBindings.interactionProfile = yvrInteractionProfilePath;
    suggestedBindings.suggestedBindings = bindings.data();
    suggestedBindings.countSuggestedBindings = static_cast<uint32_t>(bindings.size());
    OpenXRAPI(xrSuggestInteractionProfileBindings(instance, &suggestedBindings));

    XrActionSpaceCreateInfo actionSpaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
    actionSpaceInfo.action = actionCtx.poseAction;
    actionSpaceInfo.poseInActionSpace.orientation.w = 1.f;

    actionSpaceInfo.subactionPath = actionCtx.controllerSubActionPath[LEFT];
    OpenXRAPI(xrCreateActionSpace(session, &actionSpaceInfo, &controllerSpace[ControllerSide::LEFT]));

    actionSpaceInfo.subactionPath = actionCtx.controllerSubActionPath[RIGHT];
    OpenXRAPI(xrCreateActionSpace(session, &actionSpaceInfo, &controllerSpace[ControllerSide::RIGHT]));
}

void YVRControllersMgr::destroyActions()
{
    AnnounceCallingFunc();
    destroyControllerAction(actionCtx.thumbstickYValue);
    destroyControllerAction(actionCtx.thumbstickXValue);
    destroyControllerAction(actionCtx.triggerTouchAction);
    destroyControllerAction(actionCtx.triggerValue);
    destroyControllerAction(actionCtx.menuButtonAction);
    destroyControllerAction(actionCtx.gripButtonAction);
    destroyControllerAction(actionCtx.thumbstickTouchAction);
    destroyControllerAction(actionCtx.thumbstickButtonAction);
    destroyControllerAction(actionCtx.secondaryTouchAction);
    destroyControllerAction(actionCtx.secondaryButtonAction);
    destroyControllerAction(actionCtx.primaryTouchAction);
    destroyControllerAction(actionCtx.primaryButtonAction);
    destroyControllerAction(actionCtx.vibrateAction);
    destroyControllerAction(actionCtx.poseAction);
    OpenXRAPI(xrDestroyActionSet(actionCtx.actionSet));
}

void YVRControllersMgr::syncControllerChargeState()
{
    xrGetControllerChargeStatusYVR(plugin.openxrMgr->program->instance, XR_CONTROLLER_LEFT_YVR, &actionCtx.ctrlChargeStatus[LEFT]);
    xrGetControllerChargeStatusYVR(plugin.openxrMgr->program->instance, XR_CONTROLLER_RIGHT_YVR, &actionCtx.ctrlChargeStatus[RIGHT]);
}

void YVRControllersMgr::refreshControllerPose(const bool forRender)
{
    if (isControllerEnable)
    {
        const XrTime& predictedDisplayTime = plugin.inputMgr->getTime(forRender);
        const XrSpace& appSpace = plugin.inputMgr->appSpace;
        XrSpaceLocation* const leftLocation = forRender ? &leftRenderPose.location : &leftPhysicsPose.location;
        XrSpaceLocation* const rightLocation = forRender ? &rightRenderPose.location : &rightPhysicsPose.location;

        xrLocateSpace(plugin.controllersMgr->controllerSpace[LEFT], appSpace, predictedDisplayTime,
                      leftLocation);
        xrLocateSpace(plugin.controllersMgr->controllerSpace[RIGHT], appSpace, predictedDisplayTime,
                      rightLocation);
    }
}

XrControllerYVR YVRControllersMgr::getPrimaryController()
{
    XrControllerYVR ret;
    OpenXRAPI(xrGetPrimaryControllerHandleYVR(plugin.openxrMgr->program->instance,&ret));
    return ret;
}

void YVRControllersMgr::setPrimaryController(const XrControllerYVR& controller)
{
    OpenXRAPI(xrSetPrimaryControllerHandleYVR(plugin.openxrMgr->program->instance,controller));
}

void YVRControllersMgr::setControllerVibration(const bool isLeftController, const float amplitude, const float frequency, const XrDuration duration)
{
    AnnounceCallingFunc();
    if (!plugin.lifecycleMgr->isFocusing())
        YWarn("Set Vibration while not focusing");

    XrHapticVibration vibration{XR_TYPE_HAPTIC_VIBRATION};
    vibration.amplitude = amplitude;
    vibration.duration = duration == -1 ? XR_MIN_HAPTIC_DURATION : duration;
    vibration.frequency = frequency == -1 ? XR_FREQUENCY_UNSPECIFIED : frequency;

    XrHapticActionInfo hapticActionInfo{XR_TYPE_HAPTIC_ACTION_INFO};
    hapticActionInfo.action = actionCtx.vibrateAction;
    hapticActionInfo.subactionPath = actionCtx.controllerSubActionPath[isLeftController ? LEFT : RIGHT];

    OpenXRAPI(xrApplyHapticFeedback(plugin.openxrMgr->program->session, &hapticActionInfo, reinterpret_cast<XrHapticBaseHeader*>(&vibration)));
}

void YVRControllersMgr::stopControllerVibration(const bool isLeftController)
{
    AnnounceCallingFunc()
    if (!plugin.lifecycleMgr->isFocusing())
        YWarn("Set Vibration while not focusing");

    XrHapticActionInfo hapticActionInfo{XR_TYPE_HAPTIC_ACTION_INFO};
    hapticActionInfo.action = actionCtx.vibrateAction;
    hapticActionInfo.subactionPath = actionCtx.controllerSubActionPath[isLeftController ? LEFT : RIGHT];

    OpenXRAPI(xrStopHapticFeedback(plugin.openxrMgr->program->session,&hapticActionInfo));
}

void YVRControllersMgr::setControllerEnable(const bool enabled)
{
    AnnounceCalledFunc();
    isControllerEnable = enabled;
}

bool YVRControllersMgr::getBooleanActionValue(const bool isLeftController, const XrAction& action)
{
    XrActionStateBoolean state{XR_TYPE_ACTION_STATE_BOOLEAN};
    XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
    getInfo.action = action;
    getInfo.subactionPath = plugin.controllersMgr->actionCtx.controllerSubActionPath[isLeftController ? LEFT : RIGHT];

    OpenXRAPI(xrGetActionStateBoolean(plugin.openxrMgr->program->session, &getInfo, &state));
    return state.currentState == XR_TRUE;
}

float YVRControllersMgr::getFloatActionValue(const bool isLeftController, const XrAction& action)
{
    XrActionStateFloat state{XR_TYPE_ACTION_STATE_FLOAT};
    XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
    getInfo.action = action;
    getInfo.subactionPath = plugin.controllersMgr->actionCtx.controllerSubActionPath[isLeftController ? LEFT : RIGHT];

    OpenXRAPI(xrGetActionStateFloat(plugin.openxrMgr->program->session, &getInfo, &state));
    return state.currentState;
}

bool YVRControllersMgr::getActiveValue(const ControllerSide side)
{
    XrActionStatePose state{XR_TYPE_ACTION_STATE_POSE};
    XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
    getInfo.action = plugin.controllersMgr->actionCtx.poseAction;
    getInfo.subactionPath = plugin.controllersMgr->actionCtx.controllerSubActionPath[side];

    OpenXRAPI(xrGetActionStatePose(plugin.openxrMgr->program->session, &getInfo, &state));
    return state.isActive;
}

void YVRControllersMgr::createControllerAction(XrAction& action, const XrActionType type, const char* actionName, const char* localizedActionName)
{
    XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
    actionInfo.actionType = type;
    strcpy(actionInfo.actionName, actionName);
    strcpy(actionInfo.localizedActionName, localizedActionName);
    actionInfo.countSubactionPaths = static_cast<uint32_t>(actionCtx.controllerSubActionPath.size());
    actionInfo.subactionPaths = actionCtx.controllerSubActionPath.data();
    OpenXRAPI(xrCreateAction(actionCtx.actionSet, &actionInfo, &action));
}

void YVRControllersMgr::destroyControllerAction(const XrAction& action)
{
    OpenXRAPI(xrDestroyAction(action))
}
